import requests
APIKEY = "ENTER YOUR OWN APIKEY"
url = "http://maps.google.com/maps/api/staticmap?center="

# Creating the placemarks on the canvas
url += "39.1043,-108.875&zoom=10&maptype=hybrid&size=800x800"
url += "&markers=color:blue%7Clabel:S%7C"
url += "39.1043,-108.875"
url += "|39.0748,-108.923"
url += "|39.0671,-109.023"
url += "|38.881,-108.818"
# End of the placemarks section
#Beginning of the line string section
url += "&path=color:0xff0000ff|weight:2"
url += "|39.1043,-108.875"
url += "|39.0748,-108.923"
url += "|39.0671,-109.023"
url += "|38.881,-108.818"
#End of the line string section
url += "&key="
url += APIKEY
session = requests.Session()
r = session.get(url)
f = open('testMap.png','wb')
f.write(r.content)
f.close()
