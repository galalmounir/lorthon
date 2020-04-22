import lorthon
import time


if __name__ == "__main__":
    initRet = lorthon.py_LoRaInit("global_conf.json")
    if(initRet == 0):
        print("Lorthon init successful!")
        #while True:
            #value = lorthon.py_LoRaRx()
            #print(value)
            #time.sleep(1)
            #lorthon.py_LoRaTx(915000000, 0, 0, 22, 0x03, 0x02, 1, 15, False, False, "HELLO WORLD", 11)
            #time.sleep(5)
    else:
        print(f"Lorthon init Error: {initRet}")

    lorthon.py_LoRaStop()
