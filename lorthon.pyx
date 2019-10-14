cdef extern from "interface_lora.h":
	void LoRaTx( unsigned int freq, char tx_mode, unsigned int count_us,
			char rf_chain, char rf_power, char modulation, 
			char bandwidth, unsigned int datarate, char coderate,
			char invert_pol, char f_dev, unsigned short preamble, char no_crc,
			char no_header, unsigned char* payload, unsigned short size);
	char LoRaInit(const char * confFile);
	void LoRaStop();
	char* LoRaRx();

def py_LoRaTx(freq: int, tx_mode: int, count_us: int, rf_chain: int, rf_power: int, modulation: int, bandwidth: int, datarate: int, 
		coderate: int, invert_pol: bool, f_dev: int, preamble: int, no_crc: bool, no_header: bool, payload: bytes, size: int) -> None:
	LoRaTx(freq, tx_mode, count_us, rf_chain, rf_power, modulation, bandwidth, datarate, coderate, invert_pol, f_dev, preamble, no_crc, no_header, payload, size)
def py_LoRaInit(confFile):
	value = LoRaInit (confFile)
	return value
def py_LoRaStop():
	LoRaStop()
def py_LoRaRx():
	value = LoRaRx()
	return value