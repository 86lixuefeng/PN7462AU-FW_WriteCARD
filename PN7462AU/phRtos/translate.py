#
#
# Helper utility to generate cleaner RTOS Porting code
#
import sys

class RTOS_Translater(object):

    TRANSLATE_TABLE = [
        # NXP,        #Customer

        ("/* RTOS_PORTING_DISCLAIMER */","#error \"This file needs to be updated. See Accompanying RTOS Porting Guide.\""),

        ("Defines / PreProcessor Macros"),

        (" PH_UNUSED", ""),
        ("portMAX_DELAY", "MAXIMUM_DELAY"),
        ("portSTACK_TYPE", "STACK_TYPE"),
        ("pdFALSE" , "FALSE"),
        ("pdTRUE" , "TRUE"),

        # includes
        ("Header Files"),
        ("portmacro.h", "PortingMacros.h"),
        ("FreeRTOS.h", "MAIN_API.h"),
        ("task.h", "TASK_API.h"),
        ("queue.h", "QUEUE_API.h"),
        ("semphr.h", "SEMAPHORE_API.h"),
        ("event_groups.h", "EVENT_API.h"),
        ("timers.h", "TIMER_API.h"),

        ("Data Types"),
        ("BaseType_t", "DefaultType_t"),
        ("TaskHandle_t", "TaskHandleType_t"),

        ("Task/Task Scheduler  APIs"),

        ("vTaskStartScheduler", "TaskSchedulerStart"),
        ("vTaskEndScheduler", "TaskSchedulerStop"),
        ("xTaskGetSchedulerState", "TaskSchedulerGetState"),

        ("xTaskCreate", "TaskCreate"),
        ("vTaskDelete", "TaskDelete"),
        ("pcTaskGetTaskName", "TaskGetName"),
        ("vTaskDelay", "TaskDelay"),

        ("vTaskSuspend", "TaskSuspend"),
        ("vTaskSuspendAll", "TaskSuspendAll"),
        ("xTaskResumeAll", "TaskResumeAll"),

        ("xTaskResumeFromISR", "TaskResume_ISRSAFE"),
        ("vTaskResume", "TaskResume"),
        ("taskYIELD", "TaskYield"),
        ("eTaskGetState", "TaskGetState"),
        ("vTaskPrioritySet", "TaskSetPriority"),
        ("uxTaskPriorityGet", "TaskGetPriority"),
        ("uxTaskGetStackHighWaterMark", "TaskGetStackWaterMark"),
        ("portYIELD_FROM_ISR", "YIELD_FROM_ISR"),

        ("Queue APIs"),

        ("xQueueCreate", "QueueCreate"),
        ("vQueueDelete", "QueueDelete"),
        ("xQueueReset", "QueueReset"),
        ("xQueueSendFromISR", "QueuePost_ISRSAFE"),
        ("xQueueSend", "QueuePost"),
        ("xQueueReceiveFromISR", "QueueGet_ISRSAFE"),
        ("xQueueReceive", "QueueGet"),

        ("Semaphore/Mutex APIs"),

        ("xSemaphoreGiveFromISR", "SemaphoreRelease_ISRSAFE"),
        ("xSemaphoreGive", "SemaphoreRelease"),
        ("xSemaphoreGetMutexHolder", "SemaphoreGetMutexHolder"),
        ("vSemaphoreDelete", "SemaphoreDelete"),
        ("xSemaphoreCreateMutex", "SemaphoreCreate_Mutex"),
        ("xSemaphoreCreateCounting", "SemaphoreCreate_Count"),

        ("xSemaphoreTakeFromISR", "SemaphoreAcquire_ISRSAFE"),
        ("xSemaphoreTake", "SemaphoreAcquire"),

        ("Events APIs"),

        ("xEventGroupCreate", "EventBitsCreate"),
        ("xEventGroupSetBitsFromISR", "EventBitsSet_ISRSAFE"),
        ("xEventGroupSetBits", "EventBitsSet"),
        ("xEventGroupWaitBits", "EventBitsWait"),
        ("xEventGroupClearBitsFromISR", "EventBitsClear_ISRSAFE"),
        ("xEventGroupClearBits", "EventBitsClear"),
        ("vEventGroupDelete", "EventDelete"),

        ("Timer APIs"),

        ("xTimerCreate", "TimerCreate"),
        ("pvTimerGetTimerID", "TimerGetID"),
        ("xTimerStart", "TimerStart"),
        ("xTimerStop", "TimerStop"),
        ("xTimerReset", "TimerReset"),
        ("xTimerDelete", "TimerDelete"),
        ("xTimerChangePeriod", "TimerChangeDuration"),

        ("MISC APIs"),

        ("xTaskGetTickCountFromISR", "GetTickCount_ISRSAFE"),
        ("xTaskGetTickCount", "GetTickCount"),
        # ("prvDisableSysTick", "SysTickDisable"),
        # ("prvEnableSysTick", "SysTickEnable"),

        # ("portNVIC_SYSTICK_CUR_VAL", "pNVICREG_SYSTICK_CUR_VAL"),

        ("vApplicationTickHook", "CallBackOnEveryTick"),
        ("vApplicationMallocFailedHook", "NoFreeMemory"),
        ("vApplicationStackOverflowHook", "StackOverflowDetected"),

        ]

    def FromNXPToCustomer(self, inFile, outFile):
        o_in = open(inFile)
        o_out = open(outFile, "w")

        self._nxp_to_customer(o_in, o_out)
        print("Nxp=>Customer: %s => %s" % (inFile, outFile))
        o_in.close()
        o_out.close()
    def CustomerToNxp(self, inFile, outFile):
        o_in = open(inFile)
        o_out = open(outFile, "w")

        self._customer_to_nxp(o_in, o_out)
        print("Customer=>Nxp: %s => %s" % (outFile, inFile))
        o_in.close()
        o_out.close()

    def HELP(self, outFile):
        o_out = open(outFile, "w")
        for e in self.TRANSLATE_TABLE:
            if len(e) == 2:
                if len(e[1]) > 0:
                    o_out.write("EXAMPLE_RTOS_%s,%s\n" % (e[1], e[0]))
            else:
                o_out.write(">%s<\n" % (e))
        o_out.close()
    def _nxp_to_customer(self, o_in, o_out):
        for l in o_in:
            if "PHFL_ERRINJ" in l:
                pass
            else:
                for e in self.TRANSLATE_TABLE:
                    if len(e) == 2:
                        (nxp, cust) = e
                        while nxp in l:
                            if "#" in cust:
                                l = l.replace(nxp, cust)
                            else:
                                l = l.replace(nxp, "EXAMPLE_RTOS_" + cust)
                            # l = l.replace(nxp, "XXX")
                            # l = ""
                o_out.write(l)

    def _customer_to_nxp(self, o_in, o_out):
        for l in o_in:
            o_out.write(l)

def test():
    ROOT = r'C:\_ddm\n\pn640\_fl\pn640.flash_PN7462AU\pn640Flash\phRtos'

    r = RTOS_Translater()

    r.FromNXPToCustomer(
        "%s/%s" % (ROOT, "inc/wrapper/phRtos_RTOS.h"),
        "phRtos_EXAMPLE_RTOS.h")
    r.FromNXPToCustomer(
        "%s/%s" % (ROOT, "src/phRtos_RTOS.c"),
        "phRtos_EXAMPLE_RTOS.c")

    r.CustomerToNxp(
        "phRtos_EXAMPLE_RTOS.h",
        "phRtos_GENERATED_RTOS.h")
    r.CustomerToNxp(
        "phRtos_EXAMPLE_RTOS.c",
        "phRtos_GENERATED_RTOS.c")

    r.HELP("HELP.txt")

def usage():
    print("%s <NXP2CUSTOMER|CUSTOMER2NXP> <INFILE> <OUTFILE>" % (sys.argv[0]))
    print("%s <HELP> <OUTHELPDOC>" % (sys.argv[0]))

def main():
    if len(sys.argv) == 4:
        r = RTOS_Translater()
        if sys.argv[1].upper() == 'NXP2CUSTOMER':
            r.FromNXPToCustomer(sys.argv[2], sys.argv[3])
        elif sys.argv[1].upper() == 'CUSTOMER2NXP':
            r.CustomerToNxp(sys.argv[2], sys.argv[3])
        else:
            usage()
    elif len(sys.argv) == 3 and sys.argv[1].upper() == 'HELP':
        r = RTOS_Translater()
        r.HELP(sys.argv[2])
    else:
        usage()

if __name__ == '__main__':
    main()
