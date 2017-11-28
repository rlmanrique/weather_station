import serial
import calendar
import time
import argparse
import http.client, urllib.parse
import re

######################################################################################################
### --- Classes --- ###
class Context(object):
    def __init__(self):

        # Influx
        self.influx_host  = "localhost:8086"
        self.influx_table = "weather_station"
        self.influx_temp  = "temp"
        self.influx_tags  = ",device=arduino1" # Note: They have to start with ,
        
        # Serial port
        self.serial_port = "/dev/ttyUSB0"
        self.baud_rate = 9600
        
        self.temp_measure = 0.0

######################################################################################################
### --- Functions --- ###

def get_cmd_args(context):
    opts = argparse.ArgumentParser()

    opts.add_argument('--influx', '-I',
                      default = context.influx_host,
                      dest = 'influx_host',
                      help = 'Influx address:port')
    opts.add_argument('--influx-table', '-T',
                      default = context.influx_table,
                      dest = 'influx_table',
                      help = 'Influx table')
    
    opts.add_argument('--serial-port', '-SP',
                      default = context.serial_port,
                      dest = 'serial_port',
                      help = 'Serial port')
    opts.add_argument('--baud-rate', '-BR',
                      default = context.baud_rate,
                      type = int,
                      dest = 'baud_rate',
                      help = 'Baud rate')
    
    args = opts.parse_args()

    context.influx_host  = args.influx_host
    context.influx_table = args.influx_table
    context.serial_port  = args.serial_port
    context.baud_rate    = int(args.baud_rate)

    if (context.baud_rate != 9600) and (context.baud_rate != 57600):
        print("Invalid baud rate value")
        return False
    
    return True

######################################################################################################

def check_temp_measure(temp):
    pattern = re.compile("^([0-9]+).([0-9]+)$")
    return pattern.match(string)

######################################################################################################

def get_time():
    now = calendar.timegm(time.gmtime())
    now_microseconds = str(now) + '000000000'
    return now_microseconds

######################################################################################################

def read_measures(context):
    try:
        port = serial.Serial(context.serial_port, baudrate=context.baud_rate, timeout=5)
        line = port.read(5).decode("utf-8")
    except:
        return '','Error: Cannot communicate with serial port' 
    if check_temp_measure(line):
        return '','Error: Measure has an invalid format' 
    return line, ''

######################################################################################################

def send_data(context, measure):

    try:
        conn = http.client.HTTPConnection(context.influx_host)
    except:
        return 0, 'Error: unable to connect to influxDB'

    method = "POST"
    action = "/write?db=" + context.influx_table
    
    measurement_type = context.influx_temp
    tags             = context.influx_tags
    data   = measurement_type + tags + ' value=' + str(measure) + ' ' + get_time()
    
    headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}

    try:
        conn.request(method, action, data , headers)
        response = conn.getresponse()
        return response.status, response.reason
    except:
        return 0, 'Error: unable to send data to influxDB'

######################################################################################################

def main():
    context = Context()
    if not get_cmd_args(context):
        return
    
    measures, err = read_measures(context)
    if err != '':
        print("Error reading measures, err=" + err)
        return
    
    response, reason  = send_data(context, measures)
    if (response == 0) or (response != 204):
        print("Error sending measures to database, err=" + str(reason))
        return
    print("Measure "  + measure + " has been stored in the database")
    
######################################################################################################

if __name__ == "__main__":
    main()
