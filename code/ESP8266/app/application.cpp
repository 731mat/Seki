#include <user_config.h>
#include <SmingCore/SmingCore.h>
String s = "";
Timer procTimer;
Timer vypisovac;
HttpServer server;
FTPServer ftp;
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);


void onDefault(HttpRequest &request, HttpResponse &response)
{
	response.redirect("http://"+apIP.toString());
}

void onIndex(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("index.html");
	auto &vars = tmpl->variables();
	//vars["counter"] = String(counter);
	response.sendTemplate(tmpl); // this template object will be deleted automatically
}



void getData(HttpRequest &request, HttpResponse &response)
{
	response.setContentType(ContentType::HTML);
        // Use direct strings output only for small amount of data (huge memory allocation)
        response.sendString(s);
}

void startWebServer()
{
	server.listen(80);
	server.addPath("/", onIndex);
	server.addPath("/ajax", getData);
	server.setDefaultHandler(onIndex);

	Serial.println("\r\n=== WEB SERVER STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");
}

void connectOk()
{
	Serial.println("I'm CONNECTED");
	dnsServer.start(DNS_PORT, "*", apIP);
	startWebServer();
}
void testPrintf()
{
	char pomoc[100];
	pomoc[0] = '\0';
	int i;
	if(i = Serial.available()){
		Serial.readMemoryBlock(pomoc,100);
		pomoc[i] = '\0';
	}
	s += String(pomoc);
}

void vypis(){
	Serial.println(s);
}

void init()
{
	spiffs_mount(); // Mount file system, in order to work with files

	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(false); // Enable debug output to serial

	WifiStation.enable(false);
	
	WifiAccessPoint.enable(true);
	WifiAccessPoint.config("Seki", "", AUTH_OPEN);
	procTimer.initializeMs(20, testPrintf).start();
	vypisovac.initializeMs(1000, vypis).start();
	
	// Run our method when station was connected to AP
	connectOk();
}
