/*
    This file is part of JRastro.

    JRastro is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JRastro is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JRastro.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "jrst_private.h"

jrst_agent *jrst = NULL;
unsigned long long jrst_jvmid = 0;

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm,
                                    char *options,
                                    void *reserved)
{
  jint ret;

  //allocate global agent
  if (jrst != NULL){
    printf ("[jrastro] at %s, global agent is already allocated\n",
            __FUNCTION__);
    return JNI_ERR;
  }else{
    jrst = (jrst_agent*) malloc (sizeof(jrst_agent));
  }

  //get user options through environment variables
  if (getenv ("JRST_THREAD_TRACING")) jrst->thread_tracing = 1;
  if (getenv ("JRST_METHOD_TRACING")) jrst->method_tracing = 1;
  if (getenv ("JRST_MONITOR_TRACING")) jrst->monitor_tracing = 1;
  if (getenv ("JRST_GC_TRACING")) jrst->gc_tracing = 1;

  //init filtering
  jrst_filter_init();

  //get the jvmti reference, save it in the global agent
  ret = (*jvm)->GetEnv(jvm, (void*)(&jrst->jvmti), JVMTI_VERSION_1_0);
  if (ret != JNI_OK || jrst->jvmti == NULL) {
    printf("ERROR: Unable to access JVMTI Version 1 (0x%x),"
           " is your J2SE a 1.5 or newer version?"
           " JNIEnv's GetEnv() returned %d\n", JVMTI_VERSION_1, ret);
    return JNI_ERR;
  }

  //check jvmti capabilities
  jvmtiCapabilities capabilities;
  bzero (&capabilities, sizeof(jvmtiCapabilities));
  (*GET_JVMTI())->GetPotentialCapabilities(GET_JVMTI(), &capabilities);
  if (capabilities.can_signal_thread != 1 ||
      capabilities.can_get_owned_monitor_info != 1 ||
      capabilities.can_generate_exception_events != 1 ||
      capabilities.can_generate_frame_pop_events != 1 ||
      capabilities.can_generate_method_entry_events != 1 ||
      capabilities.can_generate_method_exit_events != 1 ||
      capabilities.can_generate_vm_object_alloc_events != 1 ||
      capabilities.can_generate_object_free_events != 1 ||
      capabilities.can_get_current_thread_cpu_time != 1 ||
      capabilities.can_get_thread_cpu_time != 1 ||
      capabilities.can_access_local_variables != 1 ||
      capabilities.can_generate_compiled_method_load_events != 1 ||
      capabilities.can_maintain_original_method_order != 1 ||
      capabilities.can_generate_monitor_events != 1 ||
      capabilities.can_generate_garbage_collection_events != 1 ||
      capabilities.can_generate_all_class_hook_events != 1) {
    printf("[jrastro] at %s, JVMTI capabilities don't check\n",
           __FUNCTION__);
    return JNI_ERR;
  }
  (*GET_JVMTI())->AddCapabilities(GET_JVMTI(), &capabilities);

  //set callback functions
  jvmtiEventCallbacks cb;
  bzero (&cb, sizeof(jvmtiEventCallbacks));
  cb.VMInit = &jrst_EventVMInit;
  cb.VMDeath = &jrst_EventVMDeath;
  cb.ThreadStart = &jrst_EventThreadStart;
  cb.ThreadEnd = &jrst_EventThreadEnd;
  cb.ClassFileLoadHook = &jrst_EventClassFileLoadHook;
  cb.ClassLoad = &jrst_EventClassLoad;
  cb.ClassPrepare = &jrst_EventClassPrepare;
  cb.VMStart = &jrst_EventVMStart;
  cb.Exception = &jrst_EventException;
  cb.ExceptionCatch = &jrst_EventExceptionCatch;
  cb.SingleStep = &jrst_EventSingleStep;
  cb.FramePop = &jrst_EventFramePop;
  cb.Breakpoint = &jrst_EventBreakpoint;
  cb.FieldAccess = &jrst_EventFieldAccess;
  cb.FieldModification = &jrst_EventFieldModification;
  cb.MethodEntry = &jrst_EventMethodEntry;
  cb.MethodExit = &jrst_EventMethodExit;
  cb.NativeMethodBind = &jrst_EventNativeMethodBind;
  cb.CompiledMethodLoad = &jrst_EventCompiledMethodLoad;
  cb.CompiledMethodUnload = &jrst_EventCompiledMethodUnload;
  cb.DynamicCodeGenerated = &jrst_EventDynamicCodeGenerated;
  cb.DataDumpRequest = &jrst_EventDataDumpRequest;
  cb.MonitorWait = &jrst_EventMonitorWait;
  cb.MonitorWaited = &jrst_EventMonitorWaited;
  cb.MonitorContendedEnter = &jrst_EventMonitorContendedEnter;
  cb.MonitorContendedEntered = &jrst_EventMonitorContendedEntered;
  cb.ResourceExhausted = &jrst_EventResourceExhausted;
  cb.GarbageCollectionStart = &jrst_EventGarbageCollectionStart;
  cb.GarbageCollectionFinish = &jrst_EventGarbageCollectionFinish;
  cb.ObjectFree = &jrst_EventObjectFree;
  cb.VMObjectAlloc = &jrst_EventVMObjectAlloc;
  (*GET_JVMTI())->SetEventCallbacks(GET_JVMTI(),
                                    &cb,
                                    sizeof(jvmtiEventCallbacks));

  //set notifications jrastro uses to initialize the tracing
  //other notifications are set on VM_INIT callback
  (*GET_JVMTI())->SetEventNotificationMode(GET_JVMTI(),
                                           JVMTI_ENABLE,
                                           JVMTI_EVENT_VM_INIT,
                                           NULL);
  (*GET_JVMTI())->SetEventNotificationMode(GET_JVMTI(),
                                           JVMTI_ENABLE,
                                           JVMTI_EVENT_VM_DEATH,
                                           NULL);
  (*GET_JVMTI())->SetEventNotificationMode(GET_JVMTI(),
                                           JVMTI_ENABLE,
                                           JVMTI_EVENT_CLASS_FILE_LOAD_HOOK,
                                           NULL);


  //create monitors to protect some parts of tracing
  (*GET_JVMTI())->CreateRawMonitor(GET_JVMTI(),
                                   "agent",
                                   &(jrst->monitor));
  (*GET_JVMTI())->CreateRawMonitor(GET_JVMTI(),
                                   "thread",
                                   &(jrst->monitor_thread));
  (*GET_JVMTI())->CreateRawMonitor(GET_JVMTI(),
                                   "buffer",
                                   &(jrst->monitor_buffer));
  (*GET_JVMTI())->CreateRawMonitor(GET_JVMTI(),
                                   "newArray",
                                   &(jrst->monitor_new_array));
  (*GET_JVMTI())->CreateRawMonitor(GET_JVMTI(),
                                   "tag",
                                   &(jrst->monitor_tag));
  return JNI_OK;

}

JNIEXPORT void JNICALL Agent_OnUnload(JavaVM * vm)
{
  trace_finalize_buffers();
  jrst_filter_finalize();
}
