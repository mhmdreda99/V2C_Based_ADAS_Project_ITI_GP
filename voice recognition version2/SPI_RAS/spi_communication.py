# Import the spidev module
import spidev 

# Define the SPI bus and device numbers
bus = 0
device = 1

# Create an instance of the SpiDev class
spi = spidev.SpiDev()

# Open the SPI bus with the specified bus and device numbers
spi.open(bus, device)

# Set the maximum speed of the SPI communication to 500,000 Hz
spi.max_speed_hz = 500000

# Set the SPI mode to 0 (Clock Polarity: 0, Clock Phase: 0)
spi.mode = 0


def send_spi_command(command):
    # Send an SPI command by transferring a list containing the command
    spi.xfer2([command])
    
# End of the code 