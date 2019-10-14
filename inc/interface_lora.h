/*
 * Description: LoRa Interface Header
 * Maintainer: Galal Hassan
 *
 *      ██╗███████╗███████╗████████╗██╗  ██╗███████╗   ██████╗ ██████╗  █████╗ 
 *      ██║██╔════╝██╔════╝╚══██╔══╝██║  ██║██╔════╝  ██╔════╝ ██╔══██╗██╔══██╗
 *      ██║█████╗  █████╗     ██║   ███████║█████╗    ██║  ███╗██████╔╝╚█████╔╝
 * ██   ██║██╔══╝  ██╔══╝     ██║   ██╔══██║██╔══╝    ██║   ██║██╔══██╗██╔══██╗
 * ╚█████╔╝███████╗███████╗██╗██║   ██║  ██║███████╗██╚██████╔╝██║  ██║╚█████╔╝
 *  ╚════╝ ╚══════╝╚══════╝══╝╚═╝   ╚═╝  ╚═╝╚══════╝══ ╚═════╝ ╚═╝  ╚═╝ ╚════╝                                                                                                                                                             
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
 * Copyright 2019 Galal Hassan TRL. All rights reserved.
 * Should you have any questions regarding your right to use this Software,
 * contact Galal at www.galalhassan.com.
 */

#ifndef INTERFACE_LORA_H_
#define INTERFACE_LORA_H_
/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include<loragw_hal.h>
#include<parson.h>
#include<stdint.h>
#include<stdbool.h>
/*****************************************************************************
 *                                 DEFINES
 *****************************************************************************/


/*****************************************************************************
 *                                Prototypes
 *****************************************************************************/
uint8_t LoRaInit(const char * confFile);
uint8_t LoRaTx( uint32_t freq, uint8_t tx_mode, uint32_t count_us,
			   	uint8_t rf_chain, int8_t rf_power, uint8_t modulation, 
			   	uint8_t bandwidth, uint32_t datarate, uint8_t coderate,
			   	bool invert_pol, uint8_t f_dev, uint16_t preamble, bool no_crc,
			   	bool no_header, uint8_t* payload, uint16_t size);
char* LoRaRx();  // return the number of packets
uint8_t LoRaStop();
#endif
