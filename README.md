# comp7005project2025
Make a reliable protocol over UDP and a proxy server to test it

cmd line params
client
  target-ip      IP address of server
  target-port    server port
  timeout        timeout (in seconds, float)
server
  listen-ip      IP address server is bound to
  listen-port    port number to listen on
proxy
  listen-ip      IP address proxy is bound to
  listen-port    port number to listen on
  target-ip      IP address of true server
  target-port    server port
  client-drop    client drop chance
  server-drop    server drop chance
  client-delay   client delay chance
  server-delay   server delay chance
  client-delay-time  delay time of delayed client packets
  server-delay-time  delay time of delayed server packets

default values are defined by defaults.conf
