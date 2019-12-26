/*
 * Description: LoRa Interface
 * Maintainer: Galal Hassan
 *
 *        ██╗███████╗███████╗     ████████╗██╗  ██╗███████╗         ██████╗ ██████╗  █████╗ 
 *        ██║██╔════╝██╔════╝     ╚══██╔══╝██║  ██║██╔════╝        ██╔════╝ ██╔══██╗██╔══██╗
 *        ██║█████╗  █████╗          ██║   ███████║█████╗          ██║  ███╗██████╔╝╚█████╔╝
 *   ██   ██║██╔══╝  ██╔══╝          ██║   ██╔══██║██╔══╝          ██║   ██║██╔══██╗██╔══██╗
 *   ╚█████╔╝███████╗███████╗███████╗██║   ██║  ██║███████╗███████╗╚██████╔╝██║  ██║╚█████╔╝
 *    ╚════╝ ╚══════╝╚══════╝╚══════╝╚═╝   ╚═╝  ╚═╝╚══════╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝ ╚════╝                                                                                                                                                             
 * 					               (C)2019 Galal Hassan
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * Copyright 2019 Galal Hassan. All rights reserved.
 * Should you have any questions regarding your right to use this Software,
 * contact Galal at www.galalhassan.com.
 */
/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/
//#include<parson.h>
#include<stdio.h>
#include<string.h>
#include<interface_lora.h>
/*****************************************************************************
 *                                 DEFINES
 *****************************************************************************/

/*****************************************************************************
 *                         FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
uint8_t LoRaInit(const char * confFile){
    // Parse the JSON file
    JSON_Value 				*rootVal;
    JSON_Object 			*root = NULL;
    JSON_Object 			*conf = NULL;
    JSON_Value 				*val;

    rootVal = json_parse_file_with_comments(confFile);
    root = json_value_get_object(rootVal);
    if (root == NULL) {
        printf("ERROR: %s id not a valid JSON file\n", confFile);
        return 1;
    }

    conf = json_object_get_object(root, "SX1301_conf");
    if (conf == NULL){
   		printf("ERROR: %s does not contain a JSON object named SX1301_conf\n", confFile);
        return 1; 	
    }

    // Set board params
	struct lgw_conf_board_s boardConf;
    struct lgw_conf_rxrf_s  rfConf;
    struct lgw_conf_rxif_s  ifConf;
    char param_name[32]; /* used to generate variable parameter names */
    const char *str; /* used to store string value from JSON object */

    // Get lorawan public
    memset(&boardConf, 0, sizeof boardConf); /* initialize configuration structure */
    val = json_object_get_value(conf, "lorawan_public"); /* fetch value (if possible) */
    if (json_value_get_type(val) == JSONBoolean) {
        boardConf.lorawan_public = (bool)json_value_get_boolean(val);
    } else {
        printf("WARNING: Data type for lorawan_public seems wrong, please check\n");
        boardConf.lorawan_public = false;
    }

    val = json_object_get_value(conf, "clksrc"); /* fetch value (if possible) */
    if (json_value_get_type(val) == JSONNumber) {
        boardConf.clksrc = (uint8_t)json_value_get_number(val);
    } else {
        // printf("WARNING: Data type for clksrc seems wrong, please check\n");
        boardConf.clksrc = 0;
    }
    // printf("INFO: lorawan_public %d, clksrc %d\n", boardConf.lorawan_public, boardConf.clksrc);
    /* all parameters parsed, submitting configuration to the HAL */
    if (lgw_board_setconf(boardConf) != LGW_HAL_SUCCESS) {
        printf("ERROR: Failed to configure board\n");
        return -1;
    }

    /* set configuration for RF chains */
    for (int i = 0; i < LGW_RF_CHAIN_NB; ++i) {
        memset(&rfConf, 0, sizeof(rfConf)); /* initialize configuration structure */
        sprintf(param_name, "radio_%i", i); /* compose parameter path inside JSON structure */
        val = json_object_get_value(conf, param_name); /* fetch value (if possible) */
        if (json_value_get_type(val) != JSONObject) {
            // printf("INFO: no configuration for radio %i\n", i);
            continue;
        }
        /* there is an object to configure that radio, let's parse it */
        sprintf(param_name, "radio_%i.enable", i);
        val = json_object_dotget_value(conf, param_name);
        if (json_value_get_type(val) == JSONBoolean) {
            rfConf.enable = (bool)json_value_get_boolean(val);
        } else {
            rfConf.enable = false;
        }
        if (rfConf.enable == false) { /* radio disabled, nothing else to parse */
            // printf("INFO: radio %i disabled\n", i);
        } else  { /* radio enabled, will parse the other parameters */
            snprintf(param_name, sizeof param_name, "radio_%i.freq", i);
            rfConf.freq_hz = (uint32_t)json_object_dotget_number(conf, param_name);
            snprintf(param_name, sizeof param_name, "radio_%i.rssi_offset", i);
            rfConf.rssi_offset = (float)json_object_dotget_number(conf, param_name);
            snprintf(param_name, sizeof param_name, "radio_%i.type", i);
            str = json_object_dotget_string(conf, param_name);
            if (!strncmp(str, "SX1255", 6)) {
                rfConf.type = LGW_RADIO_TYPE_SX1255;
            } else if (!strncmp(str, "SX1257", 6)) {
                rfConf.type = LGW_RADIO_TYPE_SX1257;
            } else {
                // printf("WARNING: invalid radio type: %s (should be SX1255 or SX1257)\n", str);
            }
            snprintf(param_name, sizeof param_name, "radio_%i.tx_enable", i);
            val = json_object_dotget_value(conf, param_name);
            if (json_value_get_type(val) == JSONBoolean) {
                rfConf.tx_enable = (bool)json_value_get_boolean(val);
                if (rfConf.tx_enable == true) {
                    /* tx notch filter frequency to be set */
                    snprintf(param_name, sizeof param_name, "radio_%i.tx_notch_freq", i);
                    rfConf.tx_notch_freq = (uint32_t)json_object_dotget_number(conf, param_name);
                }
            } else {
                rfConf.tx_enable = false;
            }
            // printf("INFO: radio %i enabled (type %s), center frequency %u, RSSI offset %f, tx enabled %d, tx_notch_freq %u\n", i, str, rfConf.freq_hz, rfConf.rssi_offset, rfConf.tx_enable, rfConf.tx_notch_freq);
        }

        
        
        /* all parameters parsed, submitting configuration to the HAL */
        if (lgw_rxrf_setconf(i, rfConf) != LGW_HAL_SUCCESS) {
            printf("ERROR: invalid configuration for radio %i\n", i);
            return -1;
        }
    }

    /* set configuration for LoRa multi-SF channels (bandwidth cannot be set) */
    for (int i = 0; i < LGW_MULTI_NB; ++i) {
        memset(&ifConf, 0, sizeof(ifConf)); /* initialize configuration structure */
        sprintf(param_name, "chan_multiSF_%i", i); /* compose parameter path inside JSON structure */
        val = json_object_get_value(conf, param_name); /* fetch value (if possible) */
        if (json_value_get_type(val) != JSONObject) {
            // printf("INFO: no configuration for LoRa multi-SF channel %i\n", i);
            continue;
        }
        /* there is an object to configure that LoRa multi-SF channel, let's parse it */
        sprintf(param_name, "chan_multiSF_%i.enable", i);
        val = json_object_dotget_value(conf, param_name);
        if (json_value_get_type(val) == JSONBoolean) {
            ifConf.enable = (bool)json_value_get_boolean(val);
        } else {
            ifConf.enable = false;
        }
        if (ifConf.enable == false) { /* LoRa multi-SF channel disabled, nothing else to parse */
            // printf("INFO: LoRa multi-SF channel %i disabled\n", i);
        } else  { /* LoRa multi-SF channel enabled, will parse the other parameters */
            sprintf(param_name, "chan_multiSF_%i.radio", i);
            ifConf.rf_chain = (uint32_t)json_object_dotget_number(conf, param_name);            

            sprintf(param_name, "chan_multiSF_%i.if", i);
            ifConf.freq_hz = (int32_t)json_object_dotget_number(conf, param_name);
            // TODO: handle individual SF enabling and disabling (spread_factor)
            // printf("INFO: LoRa multi-SF channel %i enabled, radio %i selected, IF %i Hz, 125 kHz bandwidth, SF 7 to 12\n", i, ifConf.rf_chain, ifConf.freq_hz);
        }
        /* all parameters parsed, submitting configuration to the HAL */
        if (lgw_rxif_setconf(i, ifConf) != LGW_HAL_SUCCESS) {
            printf("ERROR: invalid configuration for Lora multi-SF channel %i\n", i);
            return -1;
        }
    }

    /* set configuration for LoRa standard channel */
    memset(&ifConf, 0, sizeof(ifConf)); /* initialize configuration structure */
    val = json_object_get_value(conf, "chan_Lora_std"); /* fetch value (if possible) */
    if (json_value_get_type(val) != JSONObject) {
        // printf("INFO: no configuration for LoRa standard channel\n");
    } else {
        val = json_object_dotget_value(conf, "chan_Lora_std.enable");
        if (json_value_get_type(val) == JSONBoolean) {
            ifConf.enable = (bool)json_value_get_boolean(val);
        } else {
            ifConf.enable = false;
        }
        if (ifConf.enable == false) {
            // printf("INFO: LoRa standard channel %i disabled\n", i);
        } else  {
            ifConf.rf_chain = (uint32_t)json_object_dotget_number(conf, "chan_Lora_std.radio");
            ifConf.freq_hz = (int32_t)json_object_dotget_number(conf, "chan_Lora_std.if");
            switch((uint32_t)json_object_dotget_number(conf, "chan_Lora_std.bandwidth")) {
                case 500000: ifConf.bandwidth = BW_500KHZ; break;
                case 250000: ifConf.bandwidth = BW_250KHZ; break;
                case 125000: ifConf.bandwidth = BW_125KHZ; break;
                default: ifConf.bandwidth = BW_UNDEFINED;
            }
            switch((uint32_t)json_object_dotget_number(conf, "chan_Lora_std.spread_factor")) {
                case  7: ifConf.datarate = DR_LORA_SF7;  break;
                case  8: ifConf.datarate = DR_LORA_SF8;  break;
                case  9: ifConf.datarate = DR_LORA_SF9;  break;
                case 10: ifConf.datarate = DR_LORA_SF10; break;
                case 11: ifConf.datarate = DR_LORA_SF11; break;
                case 12: ifConf.datarate = DR_LORA_SF12; break;
                default: ifConf.datarate = DR_UNDEFINED;
            }

            // printf("INFO: LoRa standard channel enabled, radio %i selected, IF %i Hz, %u Hz bandwidth, SF %u\n", ifConf.rf_chain, ifConf.freq_hz, bw, sf);
        }
        if (lgw_rxif_setconf(8, ifConf) != LGW_HAL_SUCCESS) {
            printf("ERROR: invalid configuration for Lora standard channel\n");
            return -1;
        }
    }

    /* set configuration for FSK channel */
    memset(&ifConf, 0, sizeof(ifConf)); /* initialize configuration structure */
    val = json_object_get_value(conf, "chan_FSK"); /* fetch value (if possible) */
    if (json_value_get_type(val) != JSONObject) {
        // printf("INFO: no configuration for FSK channel\n");
    } else {
        val = json_object_dotget_value(conf, "chan_FSK.enable");
        if (json_value_get_type(val) == JSONBoolean) {
            ifConf.enable = (bool)json_value_get_boolean(val);
        } else {
            ifConf.enable = false;
        }
        if (ifConf.enable == false) {
            // printf("INFO: FSK channel %i disabled\n", i);
        } else  {
            ifConf.rf_chain = (uint32_t)json_object_dotget_number(conf, "chan_FSK.radio");
            ifConf.freq_hz = (int32_t)json_object_dotget_number(conf, "chan_FSK.if");
            uint32_t bw;
            bw = (uint32_t)json_object_dotget_number(conf, "chan_FSK.bandwidth");
            if      (bw <= 7800)   ifConf.bandwidth = BW_7K8HZ;
            else if (bw <= 15600)  ifConf.bandwidth = BW_15K6HZ;
            else if (bw <= 31200)  ifConf.bandwidth = BW_31K2HZ;
            else if (bw <= 62500)  ifConf.bandwidth = BW_62K5HZ;
            else if (bw <= 125000) ifConf.bandwidth = BW_125KHZ;
            else if (bw <= 250000) ifConf.bandwidth = BW_250KHZ;
            else if (bw <= 500000) ifConf.bandwidth = BW_500KHZ;
            else ifConf.bandwidth = BW_UNDEFINED;
            ifConf.datarate = (uint32_t)json_object_dotget_number(conf, "chan_FSK.datarate");
            // printf("INFO: FSK channel enabled, radio %i selected, IF %i Hz, %u Hz bandwidth, %u bps datarate\n", ifConf.rf_chain, ifConf.freq_hz, bw, ifConf.datarate);
        }
        if (lgw_rxif_setconf(9, ifConf) != LGW_HAL_SUCCESS) {
            printf("ERROR: invalid configuration for FSK channel\n");
            return -1;
        }
    }
    json_value_free(rootVal);


	// Initialize GW 
	int gwStatus = lgw_start();
	if (gwStatus == LGW_HAL_SUCCESS){
		printf("Gateway initialized\n");
		return 0;
	} else {
		printf("Gateway failed to initialize\n");
		return 1;
	}
}

uint8_t LoRaStop(){
	if (lgw_stop() == LGW_HAL_SUCCESS){
		printf("INFO: Stopped Successfully\n");
	} else {
		printf("WARNING: Failed to stop...\n");
	}
}

uint8_t LoRaTx( uint32_t freq, uint8_t txMode, uint8_t rfChain, int8_t txPower, uint8_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preamble, bool noCrc,bool noHeader, uint8_t* payload, uint16_t size){
    struct lgw_pkt_tx_s txpkt;
    memset(&txpkt, 0, sizeof(txpkt));

    txpkt.freq_hz   	= freq;
    txpkt.bandwidth 	= bandwidth;
    txpkt.datarate  	= datarate;  
    txpkt.coderate  	= coderate; 
    txpkt.preamble  	= preamble;
    txpkt.no_crc    	= noCrc; 
    txpkt.no_header 	= noHeader;
    txpkt.rf_chain  	= rfChain;
    txpkt.tx_mode 		= txMode; 
    txpkt.rf_power 		= txPower;
    txpkt.modulation	= 0x10;
    txpkt.invert_pol	= true;
    txpkt.size 			= size;
    memcpy(&txpkt.payload, payload, size);

    uint8_t status_var;
    int txret = 0;
    txret = lgw_send(txpkt); /* non-blocking scheduling of TX packet */
    if (txret == LGW_HAL_ERROR) {
        return -1;
    } else if (txret == LGW_LBT_ISSUE ) {
        printf("Failed: Not allowed (LBT)\n");
    } else {
        /* wait for packet to finish sending */
        do {
            //wait_ms(1);
            lgw_status(TX_STATUS, &status_var); /* get TX status */
        } while (status_var != TX_FREE);
    }
}

char* LoRaRx(){  // return the number of packet
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;


    struct lgw_pkt_rx_s rxpkt[16]; /* array containing up to 16 inbound packets metadata */
    
    int nb_pkt = lgw_receive(16, rxpkt);

    JSON_Value *branch = json_value_init_array();
    JSON_Array *leaves = json_value_get_array(branch);
    json_object_set_value(root_object, "packets", branch);

    if (nb_pkt > 0){
        json_object_set_number(root_object, "number_of_pkts", nb_pkt);
        for (int i = 0; i<nb_pkt; i++){
            char buff[255] = {0};
            char payload[600] = {0};
            JSON_Value *leaf_value = json_value_init_object();
            JSON_Object *leaf_object = json_value_get_object(leaf_value);

            json_object_set_number(leaf_object, "frequency", rxpkt[i].freq_hz);
            json_object_set_number(leaf_object, "if_chain", rxpkt[i].if_chain);
            json_object_set_number(leaf_object, "status", rxpkt[i].status);
            json_object_set_number(leaf_object, "count_us", rxpkt[i].count_us);
            json_object_set_number(leaf_object, "rf_chain", rxpkt[i].rf_chain);
            json_object_set_number(leaf_object, "modulation", rxpkt[i].modulation);
            json_object_set_number(leaf_object, "bandwidth", rxpkt[i].bandwidth);
            json_object_set_number(leaf_object, "datarate", rxpkt[i].datarate);
            json_object_set_number(leaf_object, "coderate", rxpkt[i].coderate);
            json_object_set_number(leaf_object, "rssi", rxpkt[i].rssi);
            json_object_set_number(leaf_object, "snr", rxpkt[i].snr);
            json_object_set_number(leaf_object, "snr_min", rxpkt[i].snr_min);
            json_object_set_number(leaf_object, "snr_max", rxpkt[i].snr_max);
            json_object_set_number(leaf_object, "crc", rxpkt[i].crc);
            json_object_set_number(leaf_object, "size", rxpkt[i].size);
            memcpy(&buff[0],&rxpkt[i].payload[0],rxpkt[i].size);
            
            for (int j = 0; j < rxpkt[i].size; j++ ){
                sprintf(&payload[j*2], "%02X", buff[j]);
            }

            json_object_set_string(leaf_object, "payload", payload);
            json_array_append_value(leaves,leaf_value);
        }
    }
    
    serialized_string = json_serialize_to_string_pretty(root_value);
    json_value_free(root_value);    
    return serialized_string;
}
