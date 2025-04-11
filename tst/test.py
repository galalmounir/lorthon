import lorthon
import time

if (lorthon.py_LoRaInit("global_conf.json") == 0):
	while True:
		value = lorthon.py_LoRaRx()
		print (value)
		time.sleep(1)
		lorthon.py_LoRaTx(915000000, 0, 0, 22, 0x03, 0x02, 1, 15, False, False, "HELLO WORLD", 11)
		time.sleep(5)
else:
	print ("ERROR!!!!")
	
lorthon.py_LoRaStop();	
