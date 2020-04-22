cdef extern from "interface_lora.h":
     void LoRaTx( unsigned int freq, char tx_mode,
                  char rf_chain, char rf_power, 
                  char bandwidth, unsigned int datarate, char coderate,
                  unsigned short preamble, char no_crc,
                  char no_header, unsigned char* payload, unsigned short size);
     char LoRaInit(const char * confFile);
     void LoRaStop();
     char* LoRaRx();

def py_LoRaTx(freq: int, tx_mode: int, rf_chain: int, rf_power: int, bandwidth: int, datarate: int, 
		coderate: int, preamble: int, no_crc: bool, no_header: bool, payload: bytes, size: int) -> None:
    LoRaTx(freq, tx_mode, rf_chain, rf_power, bandwidth, datarate, coderate, preamble, no_crc, no_header, payload, size)

def py_LoRaInit(confFile):
    confFile = bytes(confFile, 'ascii')
    value = LoRaInit (confFile)
    return value

def py_LoRaStop():
    LoRaStop()

def py_LoRaRx():
    value = LoRaRx()
    return value



