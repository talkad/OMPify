import os
import json
from tqdm import tqdm
import dask
import dask.bag as db
import preprocess
import parse_tools 
import shutil
import tempfile
import logging
import subprocess
from subprocess import Popen, PIPE
import random
import string

download_path = '/mnt/c/Users/tal74/Downloads/'
folders = ['cpp1','cpp2','cpp3','cpp4','cpp5']


logging.basicConfig(filename='llvm.log', format='%(asctime)s - %(levelname)s - %(name)s -   %(message)s',datefmt='%d/%m/%Y %H:%M:%S',level=logging.INFO)
logger = logging.getLogger(__name__)


C_TO_IR_COMMAND = lambda filename: ["clang", "-c", "-emit-llvm", "-S", "-g1", "-Oz", filename, "-std=c17", "-Xclang", "-disable-O0-optnone", "-Wno-narrowing"]
CPP_TO_IR_COMMAND =  lambda filename: ["clang++", "-c", "-emit-llvm", "-S", "-g1", "-Oz", filename, "-std=c++17", "-Xclang", "-disable-O0-optnone", "-Wno-narrowing"]
Fortran_TO_IR_COMMAND = lambda _: ["flang"] # not working

Code2IR = {
    "c": C_TO_IR_COMMAND,
    "cpp": CPP_TO_IR_COMMAND,
    "fortran": Fortran_TO_IR_COMMAND
}



class LLVMParser:
    def __init__(self, data_dir, save_dir, lang='c'):
        self.data_dir = data_dir
        self.save_dir = save_dir
        self.lang = lang

    def get_mem_usage(self, code):
        '''
        get the memory usage of a file containing @param:code
        '''
        with tempfile.NamedTemporaryFile() as temp_file:
            file_path = temp_file.name

            temp_file.write(bytes(code, 'utf-8'))
            temp_file.flush()

            mem_usage = os.path.getsize(file_path)

        return mem_usage

    def get_llvm_ir(self, code, lang):
        '''
        Create the LLVM IR of given code
        '''
        random_string = ''.join(random.choices(string.ascii_letters + string.digits, k=5))
        filename = f"code{random_string}"
        ext = 'f90' if lang=='fortran' else lang

        with open(f'{filename}.{ext}', "w") as code_f:
            code = preprocess.add_headers(code, lang=lang)
            code_f.write(code)

        subprocess.run(Code2IR[lang](f'{filename}.{ext}'), stderr=subprocess.DEVNULL)
        os.remove(f'{filename}.{ext}')
        
        llvm_ir = ''

        if os.path.exists(f'{filename}.ll'):
            with open(f'{filename}.ll', 'r') as f:
                llvm_ir = f.read()

            os.remove(f'{filename}.ll')
        
        return llvm_ir
        
    def iterate_corpus(self):
        '''
        Iterate over the HPCorpus and for each function save the following representations:
            1. username
            2. repo name
            3. path to file
            4. function name
            5. original code
            6. LLVM IR
            7. codes SHA-256
            8. memory usage 

            ---  AST - can be used to produce replaced-tokens, DFG, etc. (will be generated at training time)
        '''

        def parse_json(json_file, lang='c'):             

            # read json and create process the data
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                for line in tqdm(f):
                    js = json.loads(line.strip())

                    if 'content' not in js:
                        continue
                    
                    repo = js['repo_name'].split('/')
                    file = js['path']

                    funcs = preprocess.extract_code_struct(js['content'], lang=lang)
                    dataset = []

                    for curr_idx, func in enumerate(funcs):
                        func_name, func_code = func
                        func_code = preprocess.remove_comments(func_code)
                        # logger.info(f'parse function {func_name} at {repo} - {file}')

                        # append all function declaration into the current function code
                        code = ''
                        for _, other_func in funcs[:curr_idx]+funcs[curr_idx+1:]:
                            decl = preprocess.get_func_declaration(other_func)

                            if len(decl.split('\n')) > 2:
                                continue

                            code += decl + '\n'
                        code += func_code

                        mem_usage = self.get_mem_usage(func_code)
                        # llvm = self.get_llvm_ir(code, lang)

                        dataset.append({'username': repo[0],
                                        'repo': repo[1],
                                        'path': file,
                                        'function': func_name,
                                        'code': func_code,
                                        'llvm': '', # llvm,
                                        'hash': preprocess.get_hash(func_code),
                                        'memory': mem_usage
                        })

                    # write the dataset into json
                    with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}_{preprocess.get_filename(file.split('/')[-1])}.json"), 'w') as data_f:
                        for sample in dataset:
                            data_f.write(json.dumps(sample) + '\n')
            
        samples = os.listdir(self.data_dir)
        samples_bag = db.from_sequence(samples)
        processed_data = samples_bag.map(parse_json)
        processed_data.compute()


# parser = LLVMParser('/mnt/c/Users/tal74/Downloads/c_0', '/home/talkad/OpenMPdb/database_creator/asd/c')
# parser.iterate_corpus()

code = "\nnamespace WebCore {\n\nclass CloseWorkerGlobalScopeTask : public ScriptExecutionContext::Task {\npublic:\n    static PassOwnPtr<CloseWorkerGlobalScopeTask> create()\n    {\n        return adoptPtr(new CloseWorkerGlobalScopeTask);\n    }\n\n    virtual void performTask(ScriptExecutionContext *context)\n    {\n        ASSERT_WITH_SECURITY_IMPLICATION(context->isWorkerGlobalScope());\n        WorkerGlobalScope* workerGlobalScope = static_cast<WorkerGlobalScope*>(context);\n        \n\n        workerGlobalScope->thread()->workerReportingProxy().workerGlobalScopeClosed();\n    }\n\n    virtual bool isCleanupTask() const { return true; }\n};\n\nWorkerGlobalScope::WorkerGlobalScope(const KURL& url, const String& userAgent, PassOwnPtr<GroupSettings> settings, WorkerThread* thread, PassRefPtr<SecurityOrigin> topOrigin)\n    : m_url(url)\n    , m_userAgent(userAgent)\n    , m_groupSettings(settings)\n    , m_script(adoptPtr(new WorkerScriptController(this)))\n    , m_thread(thread)\n#if ENABLE(INSPECTOR)\n    , m_workerInspectorController(adoptPtr(new WorkerInspectorController(this)))\n#endif\n    , m_closing(false)\n    , m_eventQueue(WorkerEventQueue::create(this))\n    , m_topOrigin(topOrigin)\n{\n    setSecurityOrigin(SecurityOrigin::create(url));\n}\n\nWorkerGlobalScope::~WorkerGlobalScope()\n{\n    ASSERT(currentThread() == thread()->threadID());\n\n    \n\n    notifyObserversOfStop();\n\n    \n\n    thread()->workerReportingProxy().workerGlobalScopeDestroyed();\n}\n\nvoid WorkerGlobalScope::applyContentSecurityPolicyFromString(const String& policy, ContentSecurityPolicy::HeaderType contentSecurityPolicyType)\n{\n    setContentSecurityPolicy(ContentSecurityPolicy::create(this));\n    contentSecurityPolicy()->didReceiveHeader(policy, contentSecurityPolicyType);\n}\n\nScriptExecutionContext* WorkerGlobalScope::scriptExecutionContext() const\n{\n    return const_cast<WorkerGlobalScope*>(this);\n}\n\nconst KURL& WorkerGlobalScope::virtualURL() const\n{\n    return m_url;\n}\n\nKURL WorkerGlobalScope::virtualCompleteURL(const String& url) const\n{\n    return completeURL(url);\n}\n\nKURL WorkerGlobalScope::completeURL(const String& url) const\n{\n    \n\n    \n\n    if (url.isNull())\n        return KURL();\n    \n\n    return KURL(m_url, url);\n}\n\nString WorkerGlobalScope::userAgent(const KURL&) const\n{\n    return m_userAgent;\n}\n\nvoid WorkerGlobalScope::disableEval(const String& errorMessage)\n{\n    m_script->disableEval(errorMessage);\n}\n\nWorkerLocation* WorkerGlobalScope::location() const\n{\n    if (!m_location)\n        m_location = WorkerLocation::create(m_url);\n    return m_location.get();\n}\n\nvoid WorkerGlobalScope::close()\n{\n    if (m_closing)\n        return;\n\n    \n\n    \n\n    \n\n    m_closing = true;\n    postTask(CloseWorkerGlobalScopeTask::create());\n}\n\nWorkerNavigator* WorkerGlobalScope::navigator() const\n{\n    if (!m_navigator)\n        m_navigator = WorkerNavigator::create(m_userAgent);\n    return m_navigator.get();\n}\n\nbool WorkerGlobalScope::hasPendingActivity() const\n{\n    ActiveDOMObjectsSet::const_iterator activeObjectsEnd = activeDOMObjects().end();\n    for (ActiveDOMObjectsSet::const_iterator iter = activeDOMObjects().begin(); iter != activeObjectsEnd; ++iter) {\n        if ((*iter)->hasPendingActivity())\n            return true;\n    }\n\n    HashSet<MessagePort*>::const_iterator messagePortsEnd = messagePorts().end();\n    for (HashSet<MessagePort*>::const_iterator iter = messagePorts().begin(); iter != messagePortsEnd; ++iter) {\n        if ((*iter)->hasPendingActivity())\n            return true;\n    }\n\n    return false;\n}\n\nvoid WorkerGlobalScope::postTask(PassOwnPtr<Task> task)\n{\n    thread()->runLoop().postTask(task);\n}\n\nint WorkerGlobalScope::setTimeout(PassOwnPtr<ScheduledAction> action, int timeout)\n{\n    return DOMTimer::install(scriptExecutionContext(), action, timeout, true);\n}\n\nvoid WorkerGlobalScope::clearTimeout(int timeoutId)\n{\n    DOMTimer::removeById(scriptExecutionContext(), timeoutId);\n}\n\n#if ENABLE(INSPECTOR)\nvoid WorkerGlobalScope::clearInspector()\n{\n    m_workerInspectorController.clear();\n}\n#endif\n\nint WorkerGlobalScope::setInterval(PassOwnPtr<ScheduledAction> action, int timeout)\n{\n    return DOMTimer::install(scriptExecutionContext(), action, timeout, false);\n}\n\nvoid WorkerGlobalScope::clearInterval(int timeoutId)\n{\n    DOMTimer::removeById(scriptExecutionContext(), timeoutId);\n}\n\nvoid WorkerGlobalScope::importScripts(const Vector<String>& urls, ExceptionCode& ec)\n{\n    ASSERT(contentSecurityPolicy());\n    ec = 0;\n    Vector<String>::const_iterator urlsEnd = urls.end();\n    Vector<KURL> completedURLs;\n    for (Vector<String>::const_iterator it = urls.begin(); it != urlsEnd; ++it) {\n        const KURL& url = scriptExecutionContext()->completeURL(*it);\n        if (!url.isValid()) {\n            ec = SYNTAX_ERR;\n            return;\n        }\n        completedURLs.append(url);\n    }\n    Vector<KURL>::const_iterator end = completedURLs.end();\n\n    for (Vector<KURL>::const_iterator it = completedURLs.begin(); it != end; ++it) {\n        RefPtr<WorkerScriptLoader> scriptLoader(WorkerScriptLoader::create());\n#if PLATFORM(BLACKBERRY)\n        scriptLoader->setTargetType(ResourceRequest::TargetIsScript);\n#endif\n        scriptLoader->loadSynchronously(scriptExecutionContext(), *it, AllowCrossOriginRequests);\n\n        \n\n        if (scriptLoader->failed()) {\n            ec = XMLHttpRequestException::NETWORK_ERR;\n            return;\n        }\n\n        InspectorInstrumentation::scriptImported(scriptExecutionContext(), scriptLoader->identifier(), scriptLoader->script());\n\n        ScriptValue exception;\n        m_script->evaluate(ScriptSourceCode(scriptLoader->script(), scriptLoader->responseURL()), &exception);\n        if (!exception.hasNoValue()) {\n            m_script->setException(exception);\n            return;\n        }\n    }\n}\n\nEventTarget* WorkerGlobalScope::errorEventTarget()\n{\n    return this;\n}\n\nvoid WorkerGlobalScope::logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, int columnNumber, PassRefPtr<ScriptCallStack>)\n{\n    thread()->workerReportingProxy().postExceptionToWorkerObject(errorMessage, lineNumber, columnNumber, sourceURL);\n}\n\nvoid WorkerGlobalScope::addConsoleMessage(MessageSource source, MessageLevel level, const String& message, unsigned long requestIdentifier)\n{\n    if (!isContextThread()) {\n        postTask(AddConsoleMessageTask::create(source, level, message));\n        return;\n    }\n\n    thread()->workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, 0, 0, String());\n    addMessageToWorkerConsole(source, level, message, String(), 0, 0, 0, 0, requestIdentifier);\n}\n\nvoid WorkerGlobalScope::addMessage(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state, unsigned long requestIdentifier)\n{\n    if (!isContextThread()) {\n        postTask(AddConsoleMessageTask::create(source, level, message));\n        return;\n    }\n\n    thread()->workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, lineNumber, columnNumber, sourceURL);\n    addMessageToWorkerConsole(source, level, message, sourceURL, lineNumber, columnNumber, callStack, state, requestIdentifier);\n}\n\nvoid WorkerGlobalScope::addMessageToWorkerConsole(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state, unsigned long requestIdentifier)\n{\n    ASSERT(isContextThread());\n    if (callStack)\n        InspectorInstrumentation::addMessageToConsole(this, source, LogMessageType, level, message, callStack.get(), requestIdentifier);\n    else\n        InspectorInstrumentation::addMessageToConsole(this, source, LogMessageType, level, message, sourceURL, lineNumber, columnNumber, state, requestIdentifier);\n}\n\nbool WorkerGlobalScope::isContextThread() const\n{\n    return currentThread() == thread()->threadID();\n}\n\nbool WorkerGlobalScope::isJSExecutionForbidden() const\n{\n    return m_script->isExecutionForbidden();\n}\n\nEventTargetData* WorkerGlobalScope::eventTargetData()\n{\n    return &m_eventTargetData;\n}\n\nEventTargetData* WorkerGlobalScope::ensureEventTargetData()\n{\n    return &m_eventTargetData;\n}\n\nWorkerGlobalScope::Observer::Observer(WorkerGlobalScope* context)\n    : m_context(context)\n{\n    ASSERT(m_context && m_context->isContextThread());\n    m_context->registerObserver(this);\n}\n\nWorkerGlobalScope::Observer::~Observer()\n{\n    if (!m_context)\n        return;\n    ASSERT(m_context->isContextThread());\n    m_context->unregisterObserver(this);\n}\n\nvoid WorkerGlobalScope::Observer::stopObserving()\n{\n    if (!m_context)\n        return;\n    ASSERT(m_context->isContextThread());\n    m_context->unregisterObserver(this);\n    m_context = 0;\n}\n\nvoid WorkerGlobalScope::registerObserver(Observer* observer)\n{\n    ASSERT(observer);\n    m_workerObservers.add(observer);\n}\n\nvoid WorkerGlobalScope::unregisterObserver(Observer* observer)\n{\n    ASSERT(observer);\n    m_workerObservers.remove(observer);\n}\n\nvoid WorkerGlobalScope::notifyObserversOfStop()\n{\n    HashSet<Observer*>::iterator iter = m_workerObservers.begin();\n    while (iter != m_workerObservers.end()) {\n        WorkerGlobalScope::Observer* observer = *iter;\n        observer->stopObserving();\n        observer->notifyStop();\n        iter = m_workerObservers.begin();\n    }\n}\n\nWorkerEventQueue* WorkerGlobalScope::eventQueue() const\n{\n    return m_eventQueue.get();\n}\n\n}"
with open('code.cpp', 'w') as f:
    f.write(code)