# About
LoRthon is a Python wrapper for the LoRa Gateway project from Lora-net/lora_gateway/libloragw/

1. Tested on:
	- Raspberry Pi with a RHF0M301 from [SeeedStudio](https://www.seeedstudio.com/LoRa-LoRaWAN-Gateway-915MHz-for-Raspberry-Pi-3-p-2821.html).

	- BeagleBoneBlack with a RHF0M301 from [SeeedStudio](https://www.seeedstudio.com/LoRa-LoRaWAN-Gateway-915MHz-for-Raspberry-Pi-3-p-2821.html).

# Features
	- Transmit
	- Receive
	- Set TX and RX settings.

# Installation
Prerequisites: 
	`pip install cython`
Compiling: 
	```git clone https://github.com/galalmounir/lorthon.git```
	Run `make` to compile and generate the python library.

# Usage
Once compiled, the library will be generated inside the `output` folder. Copy the lorthon.so file to your python project directory.

# Example
###### Import the library
`import lorthon`

###### Initializing
`lorthon.py_LoRaInit("global_conf.json")`

###### Transmission
`py_LoRaTx(freq: int, tx_mode: int, rf_chain: int, rf_power: int, bandwidth: int, datarate: int, 
        coderate: int, preamble: int, no_crc: bool, no_header: bool, payload: bytes, size: int)`

`lorthon.py_LoRaTx(915000000, 0, 0, 22, 0x03, 0x02, 1, 15, False, False, "HELLO WORLD", 11)`

###### Receiption 
The `py_LoRaRx()` function returns a list of packets received in JSON format.
`packets = lorthon.py_LoRaRx()`

```JSON
{
    "packets": [
        {
            "frequency": 914800000,
            "if_chain": 1,
            "status": 16,
            "count_us": 2689979,
            "rf_chain": 0,
            "modulation": 16,
            "bandwidth": 3,
            "datarate": 2,
            "coderate": 1,
            "rssi": -66,
            "snr": 9.5,
            "snr_min": 7.75,
            "snr_max": 12.75,
            "crc": 34481,
            "size": 25,
            "payload": "030020000048454C4C4F20574F524C4400140000380C002031"
        }
    ],
    "number_of_pkts": 1
}
```

###### Test.py
```python
import lorthon
lorthon.py_LoRaInit("global_conf.json")
lorthon.py_LoRaTx(915000000, 0, 0, 22, 0x03, 0x02, 1, 15, False, False, "HELLO WORLD", 11)
lorthon.py_LoRaStop();	
```

# Raspberry Pi Notes
A recent update of the kernel of Raspberry Pis changed the Device Tree Overlay setting for the SPI and enabled by default the Chip Select Pins 7 and 8. This causes the RHF0M301 to not work properly. To fix this, you need to disable the Chip Select Pin 7 in the Device Tree Overlay settings. You can do this by adding the following line to the `/boot/firmware/config.txt` file:
([reference](https://github.com/Lora-net/lora_gateway/issues/162))
```bash
dtoverlay=spi0-cs,cs1_pin=25
```

# Contribution 
I am continuously improving on lorthon. I will always merge working bug fixes. However, if you want to add something new to the library, please create an "issue" on github for this first so we can discuss if it should end up in the library before you start implementing it.
