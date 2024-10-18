
from machine import Pin, SPI
from ST7735 import TFT,TFTColor
from framebuf import FrameBuffer, RGB565
def main():
    
    spi = SPI(2, baudrate=10000000, polarity=0, phase=0, sck=Pin(18), mosi=Pin(23))
    BL = Pin(22, Pin.OUT)    
    BL.value(1)
    
    tft=TFT(spi,2,15,5)
    tft.initr()
    tft.fill(tft.WHITE)
#     tft.image(0,0,44,44,picture.arr)


        
if __name__ == "__main__":
    main()