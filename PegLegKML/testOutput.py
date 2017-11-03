import requests
APIKEY = "ENTER YOUR OWN APIKEY"
url = "http://maps.google.com/maps/api/staticmap?&maptype=hybrid&size=640x640"

# Creating the placemarks on the canvas
# End of the placemarks section
#Beginning of the line string section
url += "&path=color:0xff0000ff|weight:2"
url += "|2,2"
#End of the line string section
url += "&key="
url += APIKEY
session = requests.Session()
r = session.get(url)
f = open('testMap.png','wb')
f.write(r.content)
f.close()
