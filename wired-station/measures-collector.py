import serial
import time
import argparse
import http.client, urllib.parse

class Context(object):
    def __init__(self):

        # Influx
        self.influx_host  = "localhost:8086"
        self.influx_table = "mydb"

        # Serial port
        self.serial_port = "/dev/ttyUSB0"
        self.baud_rate = 9600
        
        self.temp_measure = 0.0
        
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

def readlineCR(port):
    rv = ""
    while True:
        ch = port.read()
        rv += ch
        if ch=='\r' or ch=='':
            return rv

def read_measures(context):
    port = serial.Serial(context.serial_port, baudrate=context.baud_rate, timeout=2.0)
    while True:
        rcv = readlineCR(port)
        context.temp_measure = int(repr(rcv))
        return 

def prepare_data(context):
    conn = http.client.HTTPConnection(context.influx_host)
    method = "POST"
    action = "/write?db=" + context.influx_table
    data = 'cpu_load_short,host=server01,region=us-west value=0.67 14340555620000001'
    headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
    conn.request(method, action, data , headers)
    response = conn.getresponse()
    print(response.status, response.reason)
    return '',''
    
def main():
    context = Context()
    if not get_cmd_args(context):
        return
    
    measures, err = read_measures(context)
    if err != '':
        print("Error reading measures, err=" + err)
        return

    data, err = prepare_data(context)
    if err != '':
        print("Error preparing the data, err=" + err)
        return

    err = send_measure(data)
    if err != '':
        print("Error sending measures to database, err=" + err)
        return

#####################################################################################
# --- Main --- #
if __name__ == "__main__":
    main()
        
