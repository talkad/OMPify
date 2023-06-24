
namespace WebCore {

class CloseWorkerGlobalScopeTask : public ScriptExecutionContext::Task {
public:
    static PassOwnPtr<CloseWorkerGlobalScopeTask> create()
    {
        return adoptPtr(new CloseWorkerGlobalScopeTask);
    }

    virtual void performTask(ScriptExecutionContext *context)
    {
        ASSERT_WITH_SECURITY_IMPLICATION(context->isWorkerGlobalScope());
        WorkerGlobalScope* workerGlobalScope = static_cast<WorkerGlobalScope*>(context);
        

        workerGlobalScope->thread()->workerReportingProxy().workerGlobalScopeClosed();
    }

    virtual bool isCleanupTask() const { return true; }
};

WorkerGlobalScope::WorkerGlobalScope(const KURL& url, const String& userAgent, PassOwnPtr<GroupSettings> settings, WorkerThread* thread, PassRefPtr<SecurityOrigin> topOrigin)
    : m_url(url)
    , m_userAgent(userAgent)
    , m_groupSettings(settings)
    , m_script(adoptPtr(new WorkerScriptController(this)))
    , m_thread(thread)
#if ENABLE(INSPECTOR)
    , m_workerInspectorController(adoptPtr(new WorkerInspectorController(this)))
#endif
    , m_closing(false)
    , m_eventQueue(WorkerEventQueue::create(this))
    , m_topOrigin(topOrigin)
{
    setSecurityOrigin(SecurityOrigin::create(url));
}

WorkerGlobalScope::~WorkerGlobalScope()
{
    ASSERT(currentThread() == thread()->threadID());

    

    notifyObserversOfStop();

    

    thread()->workerReportingProxy().workerGlobalScopeDestroyed();
}

void WorkerGlobalScope::applyContentSecurityPolicyFromString(const String& policy, ContentSecurityPolicy::HeaderType contentSecurityPolicyType)
{
    setContentSecurityPolicy(ContentSecurityPolicy::create(this));
    contentSecurityPolicy()->didReceiveHeader(policy, contentSecurityPolicyType);
}

ScriptExecutionContext* WorkerGlobalScope::scriptExecutionContext() const
{
    return const_cast<WorkerGlobalScope*>(this);
}

const KURL& WorkerGlobalScope::virtualURL() const
{
    return m_url;
}

KURL WorkerGlobalScope::virtualCompleteURL(const String& url) const
{
    return completeURL(url);
}

KURL WorkerGlobalScope::completeURL(const String& url) const
{
    

    

    if (url.isNull())
        return KURL();
    

    return KURL(m_url, url);
}

String WorkerGlobalScope::userAgent(const KURL&) const
{
    return m_userAgent;
}

void WorkerGlobalScope::disableEval(const String& errorMessage)
{
    m_script->disableEval(errorMessage);
}

WorkerLocation* WorkerGlobalScope::location() const
{
    if (!m_location)
        m_location = WorkerLocation::create(m_url);
    return m_location.get();
}

void WorkerGlobalScope::close()
{
    if (m_closing)
        return;

    

    

    

    m_closing = true;
    postTask(CloseWorkerGlobalScopeTask::create());
}

WorkerNavigator* WorkerGlobalScope::navigator() const
{
    if (!m_navigator)
        m_navigator = WorkerNavigator::create(m_userAgent);
    return m_navigator.get();
}

bool WorkerGlobalScope::hasPendingActivity() const
{
    ActiveDOMObjectsSet::const_iterator activeObjectsEnd = activeDOMObjects().end();
    for (ActiveDOMObjectsSet::const_iterator iter = activeDOMObjects().begin(); iter != activeObjectsEnd; ++iter) {
        if ((*iter)->hasPendingActivity())
            return true;
    }

    HashSet<MessagePort*>::const_iterator messagePortsEnd = messagePorts().end();
    for (HashSet<MessagePort*>::const_iterator iter = messagePorts().begin(); iter != messagePortsEnd; ++iter) {
        if ((*iter)->hasPendingActivity())
            return true;
    }

    return false;
}

void WorkerGlobalScope::postTask(PassOwnPtr<Task> task)
{
    thread()->runLoop().postTask(task);
}

int WorkerGlobalScope::setTimeout(PassOwnPtr<ScheduledAction> action, int timeout)
{
    return DOMTimer::install(scriptExecutionContext(), action, timeout, true);
}

void WorkerGlobalScope::clearTimeout(int timeoutId)
{
    DOMTimer::removeById(scriptExecutionContext(), timeoutId);
}

#if ENABLE(INSPECTOR)
void WorkerGlobalScope::clearInspector()
{
    m_workerInspectorController.clear();
}
#endif

int WorkerGlobalScope::setInterval(PassOwnPtr<ScheduledAction> action, int timeout)
{
    return DOMTimer::install(scriptExecutionContext(), action, timeout, false);
}

void WorkerGlobalScope::clearInterval(int timeoutId)
{
    DOMTimer::removeById(scriptExecutionContext(), timeoutId);
}

void WorkerGlobalScope::importScripts(const Vector<String>& urls, ExceptionCode& ec)
{
    ASSERT(contentSecurityPolicy());
    ec = 0;
    Vector<String>::const_iterator urlsEnd = urls.end();
    Vector<KURL> completedURLs;
    for (Vector<String>::const_iterator it = urls.begin(); it != urlsEnd; ++it) {
        const KURL& url = scriptExecutionContext()->completeURL(*it);
        if (!url.isValid()) {
            ec = SYNTAX_ERR;
            return;
        }
        completedURLs.append(url);
    }
    Vector<KURL>::const_iterator end = completedURLs.end();

    for (Vector<KURL>::const_iterator it = completedURLs.begin(); it != end; ++it) {
        RefPtr<WorkerScriptLoader> scriptLoader(WorkerScriptLoader::create());
#if PLATFORM(BLACKBERRY)
        scriptLoader->setTargetType(ResourceRequest::TargetIsScript);
#endif
        scriptLoader->loadSynchronously(scriptExecutionContext(), *it, AllowCrossOriginRequests);

        

        if (scriptLoader->failed()) {
            ec = XMLHttpRequestException::NETWORK_ERR;
            return;
        }

        InspectorInstrumentation::scriptImported(scriptExecutionContext(), scriptLoader->identifier(), scriptLoader->script());

        ScriptValue exception;
        m_script->evaluate(ScriptSourceCode(scriptLoader->script(), scriptLoader->responseURL()), &exception);
        if (!exception.hasNoValue()) {
            m_script->setException(exception);
            return;
        }
    }
}

EventTarget* WorkerGlobalScope::errorEventTarget()
{
    return this;
}

void WorkerGlobalScope::logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, int columnNumber, PassRefPtr<ScriptCallStack>)
{
    thread()->workerReportingProxy().postExceptionToWorkerObject(errorMessage, lineNumber, columnNumber, sourceURL);
}

void WorkerGlobalScope::addConsoleMessage(MessageSource source, MessageLevel level, const String& message, unsigned long requestIdentifier)
{
    if (!isContextThread()) {
        postTask(AddConsoleMessageTask::create(source, level, message));
        return;
    }

    thread()->workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, 0, 0, String());
    addMessageToWorkerConsole(source, level, message, String(), 0, 0, 0, 0, requestIdentifier);
}

void WorkerGlobalScope::addMessage(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state, unsigned long requestIdentifier)
{
    if (!isContextThread()) {
        postTask(AddConsoleMessageTask::create(source, level, message));
        return;
    }

    thread()->workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, lineNumber, columnNumber, sourceURL);
    addMessageToWorkerConsole(source, level, message, sourceURL, lineNumber, columnNumber, callStack, state, requestIdentifier);
}

void WorkerGlobalScope::addMessageToWorkerConsole(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state, unsigned long requestIdentifier)
{
    ASSERT(isContextThread());
    if (callStack)
        InspectorInstrumentation::addMessageToConsole(this, source, LogMessageType, level, message, callStack.get(), requestIdentifier);
    else
        InspectorInstrumentation::addMessageToConsole(this, source, LogMessageType, level, message, sourceURL, lineNumber, columnNumber, state, requestIdentifier);
}

bool WorkerGlobalScope::isContextThread() const
{
    return currentThread() == thread()->threadID();
}

bool WorkerGlobalScope::isJSExecutionForbidden() const
{
    return m_script->isExecutionForbidden();
}

EventTargetData* WorkerGlobalScope::eventTargetData()
{
    return &m_eventTargetData;
}

EventTargetData* WorkerGlobalScope::ensureEventTargetData()
{
    return &m_eventTargetData;
}

WorkerGlobalScope::Observer::Observer(WorkerGlobalScope* context)
    : m_context(context)
{
    ASSERT(m_context && m_context->isContextThread());
    m_context->registerObserver(this);
}

WorkerGlobalScope::Observer::~Observer()
{
    if (!m_context)
        return;
    ASSERT(m_context->isContextThread());
    m_context->unregisterObserver(this);
}

void WorkerGlobalScope::Observer::stopObserving()
{
    if (!m_context)
        return;
    ASSERT(m_context->isContextThread());
    m_context->unregisterObserver(this);
    m_context = 0;
}

void WorkerGlobalScope::registerObserver(Observer* observer)
{
    ASSERT(observer);
    m_workerObservers.add(observer);
}

void WorkerGlobalScope::unregisterObserver(Observer* observer)
{
    ASSERT(observer);
    m_workerObservers.remove(observer);
}

void WorkerGlobalScope::notifyObserversOfStop()
{
    HashSet<Observer*>::iterator iter = m_workerObservers.begin();
    while (iter != m_workerObservers.end()) {
        WorkerGlobalScope::Observer* observer = *iter;
        observer->stopObserving();
        observer->notifyStop();
        iter = m_workerObservers.begin();
    }
}

WorkerEventQueue* WorkerGlobalScope::eventQueue() const
{
    return m_eventQueue.get();
}

}