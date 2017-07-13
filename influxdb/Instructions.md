# Intructions to run influxdb

Before continue reading, please take a look into:

* [Influxdb documentation](https://docs.influxdata.com/influxdb/v1.2/)
* [Influxdb docker](https://hub.docker.com/_/influxdb/)

## Get the config following the instructions in influx
## docker

Create a directory to store the data stored in influxdb
and copy the config file

Run influx 
```
docker run -p 8086:8086 \
      --name influxdb \
      -v $INFLUX_DIR:/var/lib/influxdb \
      -v $INFLUX_DIR/influxdb.conf:/etc/influxdb/influxdb.conf:ro \
	  influxdb -config /etc/influxdb/influxdb.conf 
```

## How to use influxdb via http calls.

[comment]: < This create is deprecated. TODO: Check new syntax>

Create a DB
` curl -G http://localhost:8086/query --data-urlencode "q=CREATE DATABASE mydb" `

Insert data `
curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary 'cpu_load_short,host=server01,region=us-west value=0.64 1434055562000000000'
`

## How to use influxdb with a container client

Create the client `
docker run --rm --link=influxdb -it influxdb influx -host influxdb `

And then commands like 
```
CREATE DATABASE statsdemo
SHOW DATABASES
USE statsdemo
INSERT cpu,host=serverA value=0.64
```

We can also put more data in that database with the http request:
```
curl -i -XPOST 'http://localhost:8086/write?db=statsdemo' --data-binary 'cpu,host=serverA value=`cat /proc/loadavg | cut -f1 -d" "`'
```
## Grafana

To visualize data use grafana:
``` 
docker run \
  -d \
  -p 3000:3000 \
  --name=grafana \
  -e "GF_SERVER_ROOT_URL=http://grafana.server.name" \
  -e "GF_SECURITY_ADMIN_PASSWORD=secret" \
   grafana/grafana
```

Log using admin/admin
