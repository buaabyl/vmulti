#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "vmulticlient.h"

//
// Function prototypes
//

VOID
SendHidRequests(
    pvmulti_client vmulti,
    BYTE requestType
    );

//
// Implementation
//

void
Usage(void)
{
    printf("Usage: testvmulti </multitouch | /mouse | /digitizer | /joystick | /keyboard | /message>\n");
}

INT __cdecl
main(
    int argc,
    PCHAR argv[]
    )
{
    BYTE   reportId;
    pvmulti_client vmulti;

    UNREFERENCED_PARAMETER(argv);

    //
    // Parse command line
    //

    if (argc == 1)
    {
        Usage();
        return 1;
    }
    if (strcmp(argv[1], "/multitouch") == 0)
    {
        reportId = REPORTID_MTOUCH;
    }
    else if (strcmp(argv[1], "/mouse") == 0)
    {
        reportId = REPORTID_MOUSE;
    }
    else if (strcmp(argv[1], "/digitizer") == 0)
    {
        reportId = REPORTID_DIGI;
    }
    else if (strcmp(argv[1], "/joystick") == 0)
    {
        reportId = REPORTID_JOYSTICK;
    }
    else if (strcmp(argv[1], "/keyboard") == 0)
    {
        reportId = REPORTID_KEYBOARD;
    }
    else if (strcmp(argv[1], "/message") == 0)
    {
        reportId = REPORTID_MESSAGE;
    }
    else
    {
        Usage();
        return 1;
    }

    //
    // File device
    //

    vmulti = vmulti_alloc();

    if (vmulti == NULL)
    {
        return 2;
    }

    if (!vmulti_connect(vmulti))
    {
        vmulti_free(vmulti);
        return 3;
    }

    printf("...sending request(s) to our device\n");
    SendHidRequests(vmulti, reportId);

    vmulti_disconnect(vmulti);

    vmulti_free(vmulti);

    return 0;
}

VOID
SendHidRequests(
    pvmulti_client vmulti,
    BYTE requestType
    )
{
    char buffer[1024];
    int x;
    int y;
    int i = 0;

    switch (requestType)
    {
        case REPORTID_MTOUCH:
        {
            //
            // Send the multitouch reports
            //
            
            /*
            BYTE i;
            BYTE actualCount = 4; // set whatever number you want, lower than MULTI_MAX_COUNT
            PTOUCH pTouch = (PTOUCH)malloc(actualCount * sizeof(TOUCH));

            printf("Sending multitouch report\n");
            Sleep(3000);

            for (i = 0; i < actualCount; i++)
            {
                pTouch[i].ContactID = i;
                pTouch[i].Status = MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
                pTouch[i].XValue = (i + 1) * 1000;
                pTouch[i].YValue = (i + 1) * 1500 + 5000;
                pTouch[i].Width = 20;
                pTouch[i].Height = 30;
            }

            if (!vmulti_update_multitouch(vmulti, pTouch, actualCount))
              printf("vmulti_update_multitouch TOUCH_DOWN FAILED\n");
              
            for (i = 0; i < actualCount; i++)
            {
                pTouch[i].XValue += 1000;
                pTouch[i].YValue += 1000;
            }              

            if (!vmulti_update_multitouch(vmulti, pTouch, actualCount))
                printf("vmulti_update_multitouch TOUCH_MOVE FAILED\n");

            for (i = 0; i < actualCount; i++)
              pTouch[i].Status = 0;

            if (!vmulti_update_multitouch(vmulti, pTouch, actualCount))
                printf("vmulti_update_multitouch TOUCH_UP FAILED\n");
                        
            free(pTouch);
            */

#define WIDTH  32767
#define HEIGHT 32767
            int screen_width  = GetSystemMetrics(SM_CXSCREEN);
            int screen_height = GetSystemMetrics(SM_CYSCREEN);
            int i = 0;

            //Test as one click
            BYTE actualCount = 1;
            PTOUCH pTouch = (PTOUCH)malloc(actualCount * sizeof(TOUCH));

            //Waiting
            for (i = 0;i < 5;i++) {
                printf("Wait %d/5.\n", 5 - i);
                Sleep(1 * 1000);
            }

            printf("Send to driver\n");

            pTouch[0].ContactID = 10;
            pTouch[0].Width  = 20;
            pTouch[0].Height = 30;
            pTouch[0].XValue = 20 * WIDTH / screen_width;
            pTouch[0].YValue = (screen_height - 20) * HEIGHT / screen_height;

            //TOUCHEVENTF_DOWN
            pTouch[0].Status = MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
            printf("1th [0].status = 0x%08x\n", pTouch[0].Status);
            for (i = 0;i < 1;i++) {
                pTouch[0].XValue = 21 * WIDTH / screen_width;
                pTouch[0].YValue = (screen_height - 25) * HEIGHT / screen_height;

                pTouch[0].XValue += rand() % 10 - 5;
                pTouch[0].YValue += rand() % 10 - 5;

                if (!vmulti_update_multitouch(vmulti, pTouch, actualCount)) {
                    printf("vmulti_update_multitouch TOUCH_DOWN FAILED\n");
                }
                Sleep(33);
            }

            //TOUCHEVENTF_UP
            pTouch[0].Status = 0;
            pTouch[0].XValue = 21 * WIDTH / screen_width;
            pTouch[0].YValue = (screen_height - 25) * HEIGHT / screen_height;
            printf("2nd [0].status = 0x%08x\n", pTouch[0].Status);
            if (!vmulti_update_multitouch(vmulti, pTouch, actualCount)) {
                printf("vmulti_update_multitouch TOUCH_DOWN FAILED\n");
            }

            free(pTouch);

            //Waiting
            for (i = 0;i < 5;i++) {
                printf("Wait %d/5.\n", 5 - i);
                Sleep(1 * 1000);
            }


            /*
            int i = 0;
            BYTE actualCount = 4;

            int step_x = WIDTH/400;
            int step_y = HEIGHT/400;

            PTOUCH pTouch = (PTOUCH)malloc(actualCount * sizeof(TOUCH));

            //Point 0
            pTouch[0].ContactID = 0;
            pTouch[0].Status = 
                MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
            pTouch[0].Width  = 20;
            pTouch[0].Height = 30;

            //Point 1
            pTouch[1].ContactID = 1;
            pTouch[1].Status = 
                MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
            pTouch[1].Width  = 20;
            pTouch[1].Height = 30;

            //Point 2
            pTouch[2].ContactID = 2;
            pTouch[2].Status = 
                MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
            pTouch[2].Width  = 20;
            pTouch[2].Height = 30;

            //Point 3
            pTouch[3].ContactID = 3;
            pTouch[3].Status = 
                MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
            pTouch[3].Width  = 20;
            pTouch[3].Height = 30;

            //Waiting
            Sleep(5 * 1000);

            //top-left
            pTouch[0].XValue = WIDTH/2 - WIDTH/4;
            pTouch[0].YValue = HEIGHT/2 - HEIGHT/4;

            //top-right
            pTouch[1].XValue = WIDTH/2 + WIDTH/4;
            pTouch[1].YValue = HEIGHT/2 - HEIGHT/4;

            //bottom-right
            pTouch[2].XValue = WIDTH/2 + WIDTH/4;
            pTouch[2].YValue = HEIGHT/2 + HEIGHT/4;

            //bottom-left
            pTouch[3].XValue = WIDTH/2 - WIDTH/4;
            pTouch[3].YValue = HEIGHT/2 + HEIGHT/4;

            for (i = 0;i < 100;i++) {
                //[0] [1]
                //[3] [2]

                //point 0 move right
                pTouch[0].XValue += step_x;

                //point 1 move down
                pTouch[1].YValue += step_y;

                //point 2 move left
                pTouch[2].XValue -= step_x;

                //point 3 move up
                pTouch[3].YValue -= step_y;

                if (!vmulti_update_multitouch(vmulti, pTouch, actualCount))
                //if (!vmulti_update_multitouch(vmulti, pTouch, 1))
                  printf("vmulti_update_multitouch TOUCH_DOWN FAILED\n");
                else
                    printf("Send %d\n", i);

                Sleep(100);
            }

            free(pTouch);
            */
            
            break;
        }

        case REPORTID_MOUSE:
            //
            // Send the mouse report
            //
            printf("type \"exit\" to quit\n");
            printf("type \"set x y\" to send\n");
            while (1) {
                i = 0;
                while (1) {
                    buffer[i] = (char)getchar();
                    if (buffer[i] == '\n') {
                        buffer[i] = '\0';
                        break;
                    } else {
                        i++;
                    }
                }
                if (strncmp(buffer, "exit", strlen("exit")) == 0) {
                    break;
                }

                if (strncmp(buffer, "set", strlen("set")) == 0) {
                    sscanf(buffer, "set %d %d\n", &x, &y);
                    printf("Sending mouse report\n");
                    vmulti_update_mouse(vmulti, 0,
                            (unsigned short)(x * WIDTH / 1440), 
                            (unsigned short)(y *HEIGHT / 900), 0);
                }
            }
            break;

        case REPORTID_DIGI:
            //
            // Send the digitizer reports
            //
            printf("Sending digitizer report\n");
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 1000, 10000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 1000, 12000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 1000, 14000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 1000, 16000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 1000, 18000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 1000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 2000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 3000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 3000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 3000, 15000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 3000, 10000);
            vmulti_update_digi(vmulti, 0, 3000, 10000);
            break;

        case REPORTID_JOYSTICK:
        {
            //
            // Send the joystick report
            //
            USHORT buttons = 0;
            BYTE hat = 0, x = 0, y = 128, rx = 128, ry = 64, throttle = 0;
            printf("Sending joystick report\n");
            while (1)
            {
                vmulti_update_joystick(vmulti, buttons, hat, x, y, rx, ry, throttle);
                buttons = rand() | ((rand()&1) << 15); //rand() | rand() << 15 | rand() % 4 << 30;
                hat++;
                x++;
                y++;
                rx++;
                ry--;
                throttle++;
                Sleep(10);
            }
            break;
        }
        
        case REPORTID_KEYBOARD:
        {
            //
            // Send the keyboard report
            //

            // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
            // for a list of key codes            
                        
            BYTE shiftKeys;
            BYTE keyCodes[KBD_KEY_CODES] = {0, 0, 0, 0, 0, 0};
            printf("Sending keyboard report\n");

            Sleep(3000);

            // Windows key
            shiftKeys = KBD_LGUI_BIT;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            shiftKeys = 0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            Sleep(100);
            
            // 'Hello'
            shiftKeys = KBD_LSHIFT_BIT;
            keyCodes[0] = 0x0b; // 'h'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            shiftKeys = 0;
            keyCodes[0] = 0x08; // 'e'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            keyCodes[0] = 0x0f; // 'l'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            keyCodes[0] = 0x0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            keyCodes[0] = 0x0f; // 'l'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            keyCodes[0] = 0x12; // 'o'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            keyCodes[0] = 0x0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            
            // Toggle caps lock
            keyCodes[0] = 0x39; // caps lock
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            keyCodes[0] = 0x0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            break;
        }

        case REPORTID_MESSAGE:
        {
            VMultiMessageReport report;

            printf("Writing vendor message report\n");

            memcpy(report.Message, "Hello VMulti\x00", 13);

            if (vmulti_write_message(vmulti, &report))
            {
                memset(&report, 0, sizeof(report));
                printf("Reading vendor message report\n");
                if (vmulti_read_message(vmulti, &report))
                {
                    printf("Success!\n    ");
                    printf(report.Message);
                }
            }

            break;
        }
    }
}


