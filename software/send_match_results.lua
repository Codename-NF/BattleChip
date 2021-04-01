-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
-- or if you are using another router e.g. at home, change ID and Password appropriately
SSID = "BoosterFam"
SSID_PASSWORD = "Timmy#01WF"

-- configure ESP as a station
wifi.setmode(wifi.STATION)
wifi.sta.config{ssid=SSID,pwd=SSID_PASSWORD}
wifi.sta.autoconnect(1)


-- pause for connection to take place - adjust time delay if necessary or repeat until connection made
tmr.delay(1000000) -- wait 1,000,000 us = 1 second

-- This should print 5 if connection was successful
-- print(wifi.sta.status())

-- Prints the IP given to ESP8266
-- print(wifi.sta.getip())

-- List all available wireless network ---
-- See documentation: https://nodemcu.readthedocs.io/en/master/en/modules/wifi/#wifistagetap

-- May need to specify token here
-- AUTH_TOKEN = "AC******************************36"

-- this is the web address of the server
HOST = "ec2-54-151-115-99.us-west-1.compute.amazonaws.com" 

-- The following variable defines the endpoint that we will connect to
URI = "/match"

-- builds the HTTP request to send to the server
function build_post_request(host, uri, data_table)

     arg = {}

     print("Begin Post")

     for param,value in pairs(data_table) do
          table.insert(arg, "\"" .. param .."\":"..value)
     end 

     json = "{" .. table.concat(arg, ",") .. "}"

     request = "POST "..uri.." HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: application/json\r\n"..
     "Content-Length: "..string.len(json).."\r\n"..
     "\r\n"..
     json

     print("End Post")
     print(request)
     return request
end

-- This function registers a function to echo back any response from the server, to our DE1/NIOS system 
-- or hyper-terminal (depending on what the dongle is connected to)
function display(sck,response)
     print(response)
end

-- send results to the server
function send_results(player_one, player_two, winner, player_one_score, player_two_score)

     data = {
      player_one = player_one,
      player_two = player_two,
      winner = winner,
      player_one_score = player_one_score,
      player_two_score = player_two_score
     }

     socket = net.createConnection(net.TCP,0)
     socket:on("receive",display)
     socket:connect(9091,HOST)

     socket:on("connection",function(sck)
          post_request = build_post_request(HOST,URI,data)
          sck:send(post_request)
     end)
end


-- called by the DE1, the data should be the right type

-- player_one, player_two represent the player IDs (int)
-- winner is the ID of the winning player (int)
-- player_one_score and player_two_score are the player scores (int)
function upload_match_results(player_one, player_two, winner, player_one_score, player_two_score)
  ip = wifi.sta.getip()

 if(ip==nil) then
   print("Connecting...")
 else
  -- tmr.stop(0)
  print("Connected to AP!")
  print(ip)
  -- send results to the server
  send_results(player_one, player_two, winner, player_one_score, player_two_score)

 end
end