#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>   // 使用WiFiMulti库 
#include <ESP8266WebServer.h>   // 使用WebServer库
#include <FS.h> //闪存文件系统

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
 
ESP8266WebServer esp8266_server(80);    // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
 
IPAddress local_IP(10, 0, 0, 123); // 设置ESP8266-NodeMCU联网后的IP
IPAddress gateway(10, 0, 0, 1);    // 设置网关IP（通常网关IP是WiFI路由IP）
IPAddress subnet(255, 255, 255, 0);   // 设置子网掩码
IPAddress dns(10,0,0,1);           // 设置局域网DNS的IP（通常局域网DNS的IP是WiFI路由IP）
 
void setup(){
  Serial.begin(9600);          // 启动串口通讯
  Serial.println("");
 
  pinMode(LED_BUILTIN, OUTPUT);   
  digitalWrite(LED_BUILTIN, LOW);
  
  // 设置开发板网络环境
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to Config ESP8266 IP"); 
  } 
  
  wifiMulti.addAP("Yuri_Wifi_2.4G", "netgearnb"); // 将需要连接的一系列WiFi ID和密码输入这里
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  // 尝试进行wifi连接。
  while (wifiMulti.run() != WL_CONNECTED) { 
    delay(250);
    Serial.print('.');
  }
 
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP

  if(SPIFFS.begin()){                       // 启动闪存文件系统
    Serial.println("SPIFFS Started.");
  } else {
    Serial.println("SPIFFS Failed to Start.");
  }
  
  esp8266_server.on("/LED-Control", handleLEDControl);
  esp8266_server.on("/up", handleGoUp);
  esp8266_server.on("/down", handleGoDown);
  esp8266_server.on("/right", handleGoRight);
  esp8266_server.on("/left", handleGoLeft);
  esp8266_server.on("/stop", handleStop);
  esp8266_server.on("/autoAvoid", handleAutoObstacleAvoid);
  esp8266_server.onNotFound(handleUseRequest); 
  esp8266_server.begin();                            // 启动网站服务
  Serial.println("HTTP server started");
  
}
 
void loop(void){
  esp8266_server.handleClient();                    // 检查http服务器访问
}
void handleUseRequest(){
  //获取请求资源
  String reqResource = esp8266_server.uri();
//  Serial.print("Require resource: ");
//  Serial.print(reqResource+"\n");

  //通过函数处理用户请求资源
  bool fileReadOK = handleFileRead(reqResource);
  
  //判定是否可以找到资源
  if(!fileReadOK){
    esp8266_server.send(404, "text/plain", "404 Not Found");
  }
}

bool handleFileRead(String resource) {            //处理浏览器HTTP访问

  if (resource.endsWith("/")) {                   // 如果访问地址以"/"为结尾
    resource = "/index.html";                     // 则将访问地址修改为/index.html便于SPIFFS访问
  } 
  
  String contentType = getContentType(resource);  // 获取文件类型
  
  if (SPIFFS.exists(resource)) {                     // 如果访问的文件可以在SPIFFS中找到
    File file = SPIFFS.open(resource, "r");          // 则尝试打开该文件
    esp8266_server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}

//获取文件类型
String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


// 处理/LED-Control请求  
void handleLEDControl(){
   bool ledStatus = digitalRead(LED_BUILTIN);     // 此变量用于储存LED状态     
   ledStatus == HIGH ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);  // 点亮或者熄灭LED  
     
   esp8266_server.sendHeader("Location", "/");       
   esp8266_server.send(303);  
}

void handleGoUp(){
   Serial.print("100\n");  //100 直行 go Up
   
   esp8266_server.sendHeader("Location", "/");       
   esp8266_server.send(303);  
}

void handleGoDown(){
   Serial.print("101\n");  //101 后退 go Down
   
   esp8266_server.sendHeader("Location", "/");       
   esp8266_server.send(303);  
}

void handleGoRight(){
   Serial.print("102\n");  //102 右转 go right
   
   esp8266_server.sendHeader("Location", "/");       
   esp8266_server.send(303);  
}

void handleGoLeft(){
   Serial.print("103\n");  //103 左转 go left
   
   esp8266_server.sendHeader("Location", "/");       
   esp8266_server.send(303);  
}

void handleStop(){
   Serial.print("104\n");  //104 停 stop
   
   esp8266_server.sendHeader("Location", "/");       
   esp8266_server.send(303);  
}
void handleAutoObstacleAvoid(){
  Serial.print("105\n");  //105 自动避障系统 Auto Obstacle Avoidance
   
  esp8266_server.sendHeader("Location", "/");       
  esp8266_server.send(303);  
}

//void handleRoot() {       
//  esp8266_server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
//}
