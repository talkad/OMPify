import re
import hashlib
import parse_tools

redundant_line_comments = re.compile("\/\/.*")
redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)
redundant_includes = re.compile("^\W*#\W*include.*$", re.MULTILINE)


LANG_IMPORTS = {
    "cpp": """#include <iostream>
    #include <cstdlib>
    #include <string>
    #include <vector>
    #include <fstream>
    #include <iomanip>
    #include <bits/stdc++.h>
    using namespace std;""",

    "c":"""#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <math.h>""", 

    "fortran":""
}


def get_hash(code):
    hash = hashlib.sha256(code.encode('UTF-8'))
    return hash.hexdigest()


def remove_comments(code, c_lang=True):
    code = redundant_line_comments.sub("\n", code)
    code = redundant_multiline_comments.sub("\n", code)

    return code


def remove_headers(code, c_lang=True):
    code = redundant_includes.sub("", code)
    return code


def add_headers(code, lang='c'):
    return f'{LANG_IMPORTS[lang]}\n{code}'


def node2code(code, node):

    start_byte = node.start_byte-1
    end_byte = node.end_byte
    func = code[start_byte:end_byte]

    func_name = func[:func.find('(')]
    func_name = func_name.rstrip()
    func_name = func_name[func_name.rfind(' ')+1:]

    return func_name, func


def get_functions(code, node):
    # print(node.type, node.text)
    if node.type == 'function_definition':
        func_name, code = node2code(code, node)

        return [(func_name, code)]

    nodes = []
    for child in node.children:
        nodes += get_functions(code, child)
    return nodes


def get_for_loops(code, node):
    if node.type == 'for_statement':
        return [(node, node.text)]

    nodes = []
    for child in node.children:
        nodes += get_for_loops(child)
    return nodes


def get_func_declaration(func):
    return func[:func.find(')')+1] + ';'


def get_func_calls(node, func_call=False):
	if func_call and node.type == 'identifier':
		return [node.text.decode()]

	func_calls = []
	for child in node.children:
		func_calls += get_func_calls(child, func_call=(node.type=='call_expression'))

	return func_calls


def extract_code_struct(code, traverse_func=get_functions, lang='c'):

    parser = parse_tools.parsers[lang]

    tree = parser.parse(bytes(code, 'utf8'))

    try:
        result = traverse_func(code, tree.root_node)
    except Exception as e:
        # print('extraction error: ', e)
        result = []

    return result


def get_filename(filename):
     return filename[:filename.rfind('.')]


# code = "\nnamespace WebCore {\n\nclass CloseWorkerGlobalScopeTask : public ScriptExecutionContext::Task {\npublic:\n    static PassOwnPtr<CloseWorkerGlobalScopeTask> create()\n    {\n        return adoptPtr(new CloseWorkerGlobalScopeTask);\n    }\n\n    virtual void performTask(ScriptExecutionContext *context)\n    {\n        ASSERT_WITH_SECURITY_IMPLICATION(context->isWorkerGlobalScope());\n        WorkerGlobalScope* workerGlobalScope = static_cast<WorkerGlobalScope*>(context);\n        \n\n        workerGlobalScope->thread()->workerReportingProxy().workerGlobalScopeClosed();\n    }\n\n    virtual bool isCleanupTask() const { return true; }\n};\n\nWorkerGlobalScope::WorkerGlobalScope(const KURL& url, const String& userAgent, PassOwnPtr<GroupSettings> settings, WorkerThread* thread, PassRefPtr<SecurityOrigin> topOrigin)\n    : m_url(url)\n    , m_userAgent(userAgent)\n    , m_groupSettings(settings)\n    , m_script(adoptPtr(new WorkerScriptController(this)))\n    , m_thread(thread)\n#if ENABLE(INSPECTOR)\n    , m_workerInspectorController(adoptPtr(new WorkerInspectorController(this)))\n#endif\n    , m_closing(false)\n    , m_eventQueue(WorkerEventQueue::create(this))\n    , m_topOrigin(topOrigin)\n{\n    setSecurityOrigin(SecurityOrigin::create(url));\n}\n\nWorkerGlobalScope::~WorkerGlobalScope()\n{\n    ASSERT(currentThread() == thread()->threadID());\n\n    \n\n    notifyObserversOfStop();\n\n    \n\n    thread()->workerReportingProxy().workerGlobalScopeDestroyed();\n}\n\nvoid WorkerGlobalScope::applyContentSecurityPolicyFromString(const String& policy, ContentSecurityPolicy::HeaderType contentSecurityPolicyType)\n{\n    setContentSecurityPolicy(ContentSecurityPolicy::create(this));\n    contentSecurityPolicy()->didReceiveHeader(policy, contentSecurityPolicyType);\n}\n\nScriptExecutionContext* WorkerGlobalScope::scriptExecutionContext() const\n{\n    return const_cast<WorkerGlobalScope*>(this);\n}\n\nconst KURL& WorkerGlobalScope::virtualURL() const\n{\n    return m_url;\n}\n\nKURL WorkerGlobalScope::virtualCompleteURL(const String& url) const\n{\n    return completeURL(url);\n}\n\nKURL WorkerGlobalScope::completeURL(const String& url) const\n{\n    \n\n    \n\n    if (url.isNull())\n        return KURL();\n    \n\n    return KURL(m_url, url);\n}\n\nString WorkerGlobalScope::userAgent(const KURL&) const\n{\n    return m_userAgent;\n}\n\nvoid WorkerGlobalScope::disableEval(const String& errorMessage)\n{\n    m_script->disableEval(errorMessage);\n}\n\nWorkerLocation* WorkerGlobalScope::location() const\n{\n    if (!m_location)\n        m_location = WorkerLocation::create(m_url);\n    return m_location.get();\n}\n\nvoid WorkerGlobalScope::close()\n{\n    if (m_closing)\n        return;\n\n    \n\n    \n\n    \n\n    m_closing = true;\n    postTask(CloseWorkerGlobalScopeTask::create());\n}\n\nWorkerNavigator* WorkerGlobalScope::navigator() const\n{\n    if (!m_navigator)\n        m_navigator = WorkerNavigator::create(m_userAgent);\n    return m_navigator.get();\n}\n\nbool WorkerGlobalScope::hasPendingActivity() const\n{\n    ActiveDOMObjectsSet::const_iterator activeObjectsEnd = activeDOMObjects().end();\n    for (ActiveDOMObjectsSet::const_iterator iter = activeDOMObjects().begin(); iter != activeObjectsEnd; ++iter) {\n        if ((*iter)->hasPendingActivity())\n            return true;\n    }\n\n    HashSet<MessagePort*>::const_iterator messagePortsEnd = messagePorts().end();\n    for (HashSet<MessagePort*>::const_iterator iter = messagePorts().begin(); iter != messagePortsEnd; ++iter) {\n        if ((*iter)->hasPendingActivity())\n            return true;\n    }\n\n    return false;\n}\n\nvoid WorkerGlobalScope::postTask(PassOwnPtr<Task> task)\n{\n    thread()->runLoop().postTask(task);\n}\n\nint WorkerGlobalScope::setTimeout(PassOwnPtr<ScheduledAction> action, int timeout)\n{\n    return DOMTimer::install(scriptExecutionContext(), action, timeout, true);\n}\n\nvoid WorkerGlobalScope::clearTimeout(int timeoutId)\n{\n    DOMTimer::removeById(scriptExecutionContext(), timeoutId);\n}\n\n#if ENABLE(INSPECTOR)\nvoid WorkerGlobalScope::clearInspector()\n{\n    m_workerInspectorController.clear();\n}\n#endif\n\nint WorkerGlobalScope::setInterval(PassOwnPtr<ScheduledAction> action, int timeout)\n{\n    return DOMTimer::install(scriptExecutionContext(), action, timeout, false);\n}\n\nvoid WorkerGlobalScope::clearInterval(int timeoutId)\n{\n    DOMTimer::removeById(scriptExecutionContext(), timeoutId);\n}\n\nvoid WorkerGlobalScope::importScripts(const Vector<String>& urls, ExceptionCode& ec)\n{\n    ASSERT(contentSecurityPolicy());\n    ec = 0;\n    Vector<String>::const_iterator urlsEnd = urls.end();\n    Vector<KURL> completedURLs;\n    for (Vector<String>::const_iterator it = urls.begin(); it != urlsEnd; ++it) {\n        const KURL& url = scriptExecutionContext()->completeURL(*it);\n        if (!url.isValid()) {\n            ec = SYNTAX_ERR;\n            return;\n        }\n        completedURLs.append(url);\n    }\n    Vector<KURL>::const_iterator end = completedURLs.end();\n\n    for (Vector<KURL>::const_iterator it = completedURLs.begin(); it != end; ++it) {\n        RefPtr<WorkerScriptLoader> scriptLoader(WorkerScriptLoader::create());\n#if PLATFORM(BLACKBERRY)\n        scriptLoader->setTargetType(ResourceRequest::TargetIsScript);\n#endif\n        scriptLoader->loadSynchronously(scriptExecutionContext(), *it, AllowCrossOriginRequests);\n\n        \n\n        if (scriptLoader->failed()) {\n            ec = XMLHttpRequestException::NETWORK_ERR;\n            return;\n        }\n\n        InspectorInstrumentation::scriptImported(scriptExecutionContext(), scriptLoader->identifier(), scriptLoader->script());\n\n        ScriptValue exception;\n        m_script->evaluate(ScriptSourceCode(scriptLoader->script(), scriptLoader->responseURL()), &exception);\n        if (!exception.hasNoValue()) {\n            m_script->setException(exception);\n            return;\n        }\n    }\n}\n\nEventTarget* WorkerGlobalScope::errorEventTarget()\n{\n    return this;\n}\n\nvoid WorkerGlobalScope::logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, int columnNumber, PassRefPtr<ScriptCallStack>)\n{\n    thread()->workerReportingProxy().postExceptionToWorkerObject(errorMessage, lineNumber, columnNumber, sourceURL);\n}\n\nvoid WorkerGlobalScope::addConsoleMessage(MessageSource source, MessageLevel level, const String& message, unsigned long requestIdentifier)\n{\n    if (!isContextThread()) {\n        postTask(AddConsoleMessageTask::create(source, level, message));\n        return;\n    }\n\n    thread()->workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, 0, 0, String());\n    addMessageToWorkerConsole(source, level, message, String(), 0, 0, 0, 0, requestIdentifier);\n}\n\nvoid WorkerGlobalScope::addMessage(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state, unsigned long requestIdentifier)\n{\n    if (!isContextThread()) {\n        postTask(AddConsoleMessageTask::create(source, level, message));\n        return;\n    }\n\n    thread()->workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, lineNumber, columnNumber, sourceURL);\n    addMessageToWorkerConsole(source, level, message, sourceURL, lineNumber, columnNumber, callStack, state, requestIdentifier);\n}\n\nvoid WorkerGlobalScope::addMessageToWorkerConsole(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state, unsigned long requestIdentifier)\n{\n    ASSERT(isContextThread());\n    if (callStack)\n        InspectorInstrumentation::addMessageToConsole(this, source, LogMessageType, level, message, callStack.get(), requestIdentifier);\n    else\n        InspectorInstrumentation::addMessageToConsole(this, source, LogMessageType, level, message, sourceURL, lineNumber, columnNumber, state, requestIdentifier);\n}\n\nbool WorkerGlobalScope::isContextThread() const\n{\n    return currentThread() == thread()->threadID();\n}\n\nbool WorkerGlobalScope::isJSExecutionForbidden() const\n{\n    return m_script->isExecutionForbidden();\n}\n\nEventTargetData* WorkerGlobalScope::eventTargetData()\n{\n    return &m_eventTargetData;\n}\n\nEventTargetData* WorkerGlobalScope::ensureEventTargetData()\n{\n    return &m_eventTargetData;\n}\n\nWorkerGlobalScope::Observer::Observer(WorkerGlobalScope* context)\n    : m_context(context)\n{\n    ASSERT(m_context && m_context->isContextThread());\n    m_context->registerObserver(this);\n}\n\nWorkerGlobalScope::Observer::~Observer()\n{\n    if (!m_context)\n        return;\n    ASSERT(m_context->isContextThread());\n    m_context->unregisterObserver(this);\n}\n\nvoid WorkerGlobalScope::Observer::stopObserving()\n{\n    if (!m_context)\n        return;\n    ASSERT(m_context->isContextThread());\n    m_context->unregisterObserver(this);\n    m_context = 0;\n}\n\nvoid WorkerGlobalScope::registerObserver(Observer* observer)\n{\n    ASSERT(observer);\n    m_workerObservers.add(observer);\n}\n\nvoid WorkerGlobalScope::unregisterObserver(Observer* observer)\n{\n    ASSERT(observer);\n    m_workerObservers.remove(observer);\n}\n\nvoid WorkerGlobalScope::notifyObserversOfStop()\n{\n    HashSet<Observer*>::iterator iter = m_workerObservers.begin();\n    while (iter != m_workerObservers.end()) {\n        WorkerGlobalScope::Observer* observer = *iter;\n        observer->stopObserving();\n        observer->notifyStop();\n        iter = m_workerObservers.begin();\n    }\n}\n\nWorkerEventQueue* WorkerGlobalScope::eventQueue() const\n{\n    return m_eventQueue.get();\n}\n\n}"
# print(len(extract_code_struct(code, lang='cpp')))