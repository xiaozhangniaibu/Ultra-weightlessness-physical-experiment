#include <HX711.h>       //加载HX711的库函数
#include <phyphoxBle.h>  //加载phyphoxble.h库函数，此库来自https://github.com/phyphox/phyphox-arduino
HX711 hx(16, 17);        //HX711的CK、DO连接ESP32上的端口
uint8_t Zero_pin = 18;    //清零按钮
int zero_num = 1;         //按钮没有按下时值为1
float force, force_b, force_n;  //定义一些变量
double sum = 0;
float readforce()
{
  float rdforce;
  sum = 0;
  for (int i = 0; i < 4; i++)       //先读取8个数据
  {
    sum += hx.read();
  }
  rdforce = sum / 4;                //清零用的数据
  return (rdforce);
}
void setup() {
  pinMode(Zero_pin, INPUT_PULLUP);   //将清零按钮端设置成上拉，也就是端口空置时为高电压，将它与GND短接时为低电压
  Serial.begin(115200);
  PhyphoxBLE::start();  //蓝牙搜索时出现的名字
  force_b = readforce();                //清零用的数据
}
void loop() {
  zero_num = digitalRead(Zero_pin);     //读取清零按钮
  if (zero_num == 0)                    //如果按钮按下
  {
    force_b = readforce();
  }
  force_n = readforce();                //读取力传感器数据
  force = (force_n - force_b) / 391541;          //将来自HX711的数据转换成牛顿单位，后面的数据需要自己通过砝码来校准
  delay(1);
  PhyphoxBLE::write(force);                     //将力通过蓝牙输出
  Serial.println(force);                       //同时串口也输出力，以便在电脑串口监视器里查看
  delay(10);
}
