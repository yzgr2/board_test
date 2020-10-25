#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <string.h>
#include <stdio.h>
#include "structures.h"


//==================
#include <math.h>
 
#define pi 3.1415926
 
typedef struct { 
	float re;// really 
	float im;// imaginary 
}complex,*pcomplex; 

complex complexadd(complex a, complex b){ //复数加
    complex rt;
    rt.re = a.re + b.re;
    rt.im = a.im + b.im;
    return rt;
}
 
complex complexMult(complex a, complex b){ //复数乘
    complex rt;
    rt.re = a.re*b.re-a.im*b.im;
    rt.im = a.im*b.re+a.re*b.im;
    return rt;
}
//离散傅里叶变换
void dft(complex X[], complex x[], int N){ //X[]标识变换后频域，x[]为时域采样信号，下同
    complex temp;
    
	//int k, n;
    for (int k = 0; k < N; k++)
    {
        X[k].re = 0;
        X[k].im = 0;
        for (int n = 0; n < N; n++)
        {
            temp.re = (float)cos(2*pi*k*n/N);
            temp.im = -(float)sin(2*pi*k*n/N);
            X[k] = complexadd(X[k], complexMult(x[n],temp));
 
        }
         
    }
}
//离散傅里叶逆变换
void idft(complex X[], complex x[], int N){
    complex temp;
    //int k, n;
    for (int k = 0; k < N; k++)
    {
        x[k].re = 0;
        x[k].im = 0;
        for (int n = 0; n < N; n++)
        {
            temp.re = (float)cos(2*pi*k*n/N);
            temp.im = (float)sin(2*pi*k*n/N);
            x[k] = complexadd(x[k], complexMult(X[n],temp));
 
        }
        x[k].re /= N;
        x[k].im /= N;
    }
}

float abs_complex(complex *d)
{
	return sqrt( d->re*d->re + d->im*d->im );
}

void print_array_complex(complex *ori, int N)
{
	// printf("[");
    // for( int i=0; i<N; i++) {
    //     printf(" %f + (%fj), ", ori[i].re, ori[i].im);
    // }
	// printf("]\n");

	printf("[");
    for( int i=0; i<N; i++) {
        printf(" %.1f, ", abs_complex(ori + i));
    }
	printf("],\n");
}

///////////////////

#define MAC_ADD_LEN 20

/*Set the SSID and Password via "make menuconfig"*/
#define DEFAULT_SSID CONFIG_WIFI_SSID
#define DEFAULT_PWD CONFIG_WIFI_PASSWORD

#if CONFIG_WIFI_ALL_CHANNEL_SCAN
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#elif CONFIG_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif /*CONFIG_SCAN_METHOD*/

#if CONFIG_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif /*CONFIG_SORT_METHOD*/

#if CONFIG_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_FAST_SCAN_MINIMUM_SIGNAL
#if CONFIG_EXAMPLE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_EXAMPLE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_EXAMPLE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_EXAMPLE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif /*CONFIG_FAST_SCAN_THRESHOLD*/

static const char *TAG = "scan";
static bool Print_Info = 1;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id)
	{
	case SYSTEM_EVENT_STA_START:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
		// ESP_ERROR_CHECK(esp_wifi_connect());
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
		ESP_LOGI(TAG, "Got IP: %s\n",
				 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
		// ESP_ERROR_CHECK(esp_wifi_connect());
		break;
	default:
		break;
	}
	return ESP_OK;
}

//LL_Family
// char monitor_mac[] = "\x80\x8F\x1D\xD6\x94\x50";

//
char monitor_mac[] = "\xCC\x44\x63\x4E\xF3\x01";


int csi_cnt = 0;
/*
 * This function ONLY receive the CSI preamble of frames (if there is any)
 */
void receive_csi_cb(void *ctx, wifi_csi_info_t *Rx_Data)
{
	// Goal : Get Channel State Information Packets and fill fields accordingly In : Contexte (null), CSI packet  Out : Null, Fill fields of corresponding AP
	wifi_csi_info_t Rx = Rx_Data[0];

	char MacAddTx[MAC_ADD_LEN] = {0}; // Sender

	csi_cnt++;
	if ((csi_cnt & 0x7F) == 0)
	{
		printf("get csi cnt=%d channel=%d \n", csi_cnt, Rx.rx_ctrl.channel);
	}

	memcpy(MacAddTx, Rx.mac, 6);

	// printf("from_mac: %02X:%02X:%02X:%02X:%02X:%02X\n",
	//  		Rx.mac[0], Rx.mac[1], Rx.mac[2], Rx.mac[3], Rx.mac[4], Rx.mac[5]);
	

	if (memcmp(Rx_Data->mac, monitor_mac, 6))
	{
		printf("not monitored mac address. ignore\n");
		return; //not from monitor_mac.
	}

	bool dsplay = 1;
	if (dsplay)
	{
		printf("sig_mode=%d, Rate=%d, RSSI=%d cwb=%d stbc=%d datlen=%d firstword_invalid=%d channel=%d\n",
			   Rx.rx_ctrl.sig_mode, Rx.rx_ctrl.rate, Rx.rx_ctrl.rssi, Rx.rx_ctrl.cwb,
			   Rx.rx_ctrl.stbc, Rx_Data->len, Rx.first_word_invalid, Rx.rx_ctrl.channel);

		// if (Rx.rx_ctrl.sig_mode == 0)
		// 	printf("non HT(11bg)\n");
		// if (Rx.rx_ctrl.sig_mode == 1)
		// 	printf("HT(11n)\n");
		// if (Rx.rx_ctrl.sig_mode == 2)
		// 	printf("UNKNOWN!!!");
		// if (Rx.rx_ctrl.sig_mode == 3)
		// 	printf("VHT(11ac)\n");

		// printf("HT20 (0) or HT40 (1): %d\n", Rx.rx_ctrl.cwb);
		// printf("Space time block present: %d\n", Rx.rx_ctrl.stbc);
		// printf("Secondary channel : 0: none; 1: above; 2: below: %d\n", Rx.rx_ctrl.secondary_channel);
		// printf("Length %d\n", Rx.len);
		// printf("Last word is invalid: %d\n", Rx.rx_ctrl.rx_state);
		// printf("First four bytes Invalid or not: %d\n", Rx.first_word_invalid);
		// printf("<CSI><address>%s</address><len>%d</len>", MacAddTx, Rx_Data->len);
	}

	if( Rx_Data->len != 128 ) {
		return;
	}

	complex *freq_complex = malloc(sizeof(complex) * 128);
	if( !freq_complex ) {
		printf("oom  freq_complex");
		return ;
	}

	complex *time_complex = malloc(sizeof(complex) * 128);
	if( !time_complex ) {
		printf("oom  time_complex");
		return ;
	}

	int offset = 0;
	if (Rx_Data->first_word_invalid)
	{
		offset = 4;
	}

	//printf("[");
	for (int i = offset; i < Rx_Data->len; i += 2)
	{
		//printf(" %d + (%dj) ,", Rx_Data->buf[i + 1], Rx_Data->buf[i]);
		freq_complex[ (i-offset)/2 ].re = Rx_Data->buf[i];
		freq_complex[ (i-offset)/2 ].im = Rx_Data->buf[i + 1];
	}
	//printf("]\n");

	//do fft.
	idft(freq_complex, time_complex, 62);

	print_array_complex(time_complex, 62);

	free(freq_complex);
	free(time_complex);

	// vTaskDelay(1);
}

/*
 * This function receive all frames, would they contain CSI preamble or not.
 * It gets the content of the frame, not the preamble.
 */

void promi_cb(void *buff, wifi_promiscuous_pkt_type_t type)
{
	//printf("get promisc frame\n");

	// if (Print_Info)
	// {
	// 	Print_Info = 0;
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	const wifi_promiscuous_pkt_t *PayloadPacket = (wifi_promiscuous_pkt_t *)buff;
	// 	const wifi_ieee80211_packet_t *InfoPacket = (wifi_ieee80211_packet_t *)PayloadPacket->payload;
	// 	const wifi_ieee80211_mac_hdr_t *Header = &InfoPacket->hdr;
	// 	uint8_t *data_ptr = InfoPacket;
	// 	char MacAddTx[MAC_ADD_LEN] = {0};
	// 	char MacAddRx[MAC_ADD_LEN] = {0};
	// 	///////////////////////////////////////////////////////////////////////////////////////////////

	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	sprintf(MacAddRx, "%02X:%02X:%02X:%02X:%02X:%02X", Header->addr1[0], Header->addr1[1], Header->addr1[2], Header->addr1[3], Header->addr1[4], Header->addr1[5]);
	// 	sprintf(MacAddTx, "%02X:%02X:%02X:%02X:%02X:%02X", Header->addr2[0], Header->addr2[1], Header->addr2[2], Header->addr2[3], Header->addr2[4], Header->addr2[5]);
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// }
	// Print_Info = 1;
}

void app_main()
{
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	sleep(1);

	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	///////////////////////////////////////////////////////////////////////////////////////////////
	wifi_init_config_t Configuration = WIFI_INIT_CONFIG_DEFAULT();
	Configuration.csi_enable = 1;

	ESP_ERROR_CHECK(esp_wifi_init(&Configuration));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));	
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	esp_wifi_start();

	///////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
#if 0
	// wifi_promiscuous_filter_t Promicus_Filter;
	// wifi_promiscuous_filter_t Promicus_Filter_Crl;

	// uint32_t filter_promi_field = WIFI_PROMIS_FILTER_MASK_ALL;
	// uint32_t filter_promi_ctrl_field = (0xFF800000);
	// uint32_t filter_event = WIFI_EVENT_MASK_ALL;

	// Promicus_Filter.filter_mask = filter_promi_field;
	// Promicus_Filter_Crl.filter_mask = filter_promi_ctrl_field;

	// esp_wifi_set_promiscuous_filter(&Promicus_Filter);
	// esp_wifi_set_event_mask(filter_event);
	// esp_wifi_set_promiscuous_ctrl_filter(&Promicus_Filter_Crl);
#endif

	ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
	esp_wifi_set_promiscuous_rx_cb(promi_cb);
	
	///////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	//CSI Configuration
	//ESP32 offer some settings which can be configured Before Capturing
	wifi_csi_config_t CSI_Config = {0};	  // CSI = Channel State Information
	CSI_Config.lltf_en = 1;			  //1
	CSI_Config.htltf_en = 1;		  //1
	CSI_Config.stbc_htltf2_en = 1;	  //1
	CSI_Config.ltf_merge_en = 1;	  //1
	CSI_Config.channel_filter_en = 0; //1
	CSI_Config.manu_scale = 0;		  // Automatic scalling
	//CSI_Config.shift=15; // 0->15
	ESP_ERROR_CHECK(esp_wifi_set_csi_config(&CSI_Config));
	ESP_ERROR_CHECK(esp_wifi_set_csi_rx_cb(receive_csi_cb, NULL));

	ESP_ERROR_CHECK(esp_wifi_set_csi(true));  //start csi

	///////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	// AP Configrations are required to acquire CSI Packets from the RF Cahnnel
	// wifi_config_t AP_Configuration;
	// ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_AP, &AP_Configuration));
	// ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &AP_Configuration));
	// ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
	///////////////////////////////////////////////////////////////////////////////////////////////

	esp_log_level_set("TEST", ESP_LOG_VERBOSE);
	sleep(1);

	int WiFi_Channel;
	WiFi_Channel = 4;
	//There are 14 channel on WiFi RF Interface. Select that which is configured on the AP
	//int bandwith = 0;
	//
	int Ch_HighThrogh;
	// There are three possible in case of Channel This is usuallu used in case of Channel width is 40 MHz
	//WIFI_SECOND_CHAN_NONE;//WIFI_SECOND_CHAN_ABOVE or WIFI_SECOND_CHAN_BELOW
	Ch_HighThrogh = WIFI_SECOND_CHAN_NONE;

	ret = esp_wifi_set_channel(WiFi_Channel, Ch_HighThrogh);
	//ESP_ERROR_CHECK(ret);

	esp_wifi_set_ps(WIFI_PS_NONE);

	uint8_t primary = 0;
	int second = 0;
	esp_wifi_get_channel(&primary, &second);

	printf("Configuration Complete, channel=%d...\n", primary);
}
