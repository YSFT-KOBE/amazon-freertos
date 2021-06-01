/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/* Called from all the example projects to start tasks that demonstrate Amazon
 * FreeRTOS libraries.
 *
 * If the project was created using the AWS console then this file will have been
 * auto generated and only reference and start the demos that were selected in the
 * console.  If the project was obtained from a source control repository then this
 * file will reference all the available and the developer can selectively comment
 * in or out the demos to execute. */

/* The config header is always included first. */
#include "iot_config.h"

/* Includes for library initialization. */
#include "iot_demo_runner.h"
#include "platform/iot_threads.h"
#include "types/iot_network_types.h"

#include "aws_demo.h"
#include "aws_demo_config.h"

#include "driver/gpio.h"
#include "hal/gpio_types.h"

/* Forward declaration of demo entry function to be renamed from #define in
 * aws_demo_config.h */
int DEMO_entryFUNCTION( bool awsIotMqttMode,
                        const char * pIdentifier,
                        void * pNetworkServerInfo,
                        void * pNetworkCredentialInfo,
                        const IotNetworkInterface_t * pNetworkInterface );


/* Forward declaration of network connected DEMO callback to be renamed from
 * #define in aws_demo_config.h */
#ifdef DEMO_networkConnectedCallback
    void DEMO_networkConnectedCallback( bool awsIotMqttMode,
                                        const char * pIdentifier,
                                        void * pNetworkServerInfo,
                                        void * pNetworkCredentialInfo,
                                        const IotNetworkInterface_t * pNetworkInterface );
#else
    #define DEMO_networkConnectedCallback    ( NULL )
#endif


/* Forward declaration of network disconnected DEMO callback to be renamed from #define in aws_demo_config.h */
#ifdef DEMO_networkDisconnectedCallback
    void DEMO_networkDisconnectedCallback( const IotNetworkInterface_t * pNetworkInterface );
#else
    #define DEMO_networkDisconnectedCallback    ( NULL )
#endif

void TaskMainChimeGPIO(void* pParam);

/*-----------------------------------------------------------*/

/**
 * @brief Runs the one demo configured in the config file.
 */
void DEMO_RUNNER_RunDemos( void )
{
    /* These demos are shared with the C SDK and perform their own initialization and cleanup. */

    static demoContext_t mqttDemoContext =
    {
        .networkTypes                = democonfigNETWORK_TYPES,
        .demoFunction                = DEMO_entryFUNCTION,
        .networkConnectedCallback    = DEMO_networkConnectedCallback,
        .networkDisconnectedCallback = DEMO_networkDisconnectedCallback
    };

    gpio_config_t gpioP2Config ={
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 0x0000000000000004,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config_t gpioP0Config ={
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_DEF_OUTPUT,
        .pin_bit_mask = 0x0000000000000001,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&gpioP0Config);
    gpio_config(&gpioP2Config);

    Iot_CreateDetachedThread( runDemoTask,
                              &mqttDemoContext,
                              democonfigDEMO_PRIORITY,
                              democonfigDEMO_STACKSIZE );

    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    TaskMainChimeGPIO, /* Function that implements the task. */
                    "NAME",            /* Text name for the task. */
                    democonfigDEMO_STACKSIZE,        /* Stack size in words, not bytes. */
                    ( void * ) 1,      /* Parameter passed into the task. */
                    democonfigDEMO_PRIORITY,  /* Priority at which the task is created. */
                    &xHandle );        /* Used to pass out the created task's handle. */
}
