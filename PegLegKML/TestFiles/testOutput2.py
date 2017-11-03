import requests
APIKEY = "ENTER YOUR OWN APIKEY"
url = "http://maps.google.com/maps/api/staticmap?&maptype=hybrid&size=640x640"

# Creating the placemarks on the canvas
url += "&markers=color:blue%7Clabel:S%7C"
url += "38.9393,-109.612"
url += "|38.8725,-109.552"
url += "|38.8506,-109.511"
url += "|38.8243,-109.497"
url += "|38.8018,-109.516"
# End of the placemarks section
#Beginning of the line string section
url += "&path=color:0xff0000ff|weight:2"
url += "|38.9393,-109.612"
url += "|38.8725,-109.552"
url += "|38.8647,-109.55"
url += "|38.8516,-109.54"
url += "|38.8476,-109.526"
url += "|38.8506,-109.511"
url += "|38.8384,-109.513"
url += "|38.8362,-109.504"
url += "|38.8243,-109.497"
url += "|38.8018,-109.516"
#End of the line string section
url += "&key="
url += APIKEY
session = requests.Session()
r = session.get(url)
f = open('testMap.png','wb')
f.write(r.content)
f.close()
