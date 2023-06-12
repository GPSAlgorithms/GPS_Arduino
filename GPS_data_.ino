#include <SoftwareSerial.h>

SoftwareSerial GPS (4, 6);
// tx, rx
char data ="";  // \n 구분, gpsstr에 저장
String gpsstr = ""; // \n 전까지의  c값 저장
String targetStr = "GPGGA"; 
String targetStr1 = "GPRMC";
// str값이 GPGGA, GPRMC(NMEA프로토콜)의 값인지 타겟
uint8_t s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
String Lat, Long, Lat1, Long1, Lat2, Long2;
double Lat_;
float Long_;
String Date, Time;
String Year, Mon, Day, Hour, Min, Sec;
int Day_, Hour_, Min_, Sec_;

void setup()
{
  Serial.begin(9600);
  Serial.println("** GPS Start **");
  GPS.begin(9600);
}

void loop() 
{
  if(GPS.available()) // gps 센서 통신 가능 시
  {
    data=GPS.read(); // gps 센서 값 읽기
    if(data=='\n') // \n 구분
    {
      if(targetStr.equals(gpsstr.substring(1,6))) 
      // 지금까지 저장된 str값이 targetStr 값과 동일한지 확인
      {
        Serial.println(gpsstr);
        // , : 토큰
        s1 = gpsstr.indexOf(","); // 첫 번째 , 전까지의 내용 파싱
        s2 = gpsstr.indexOf(",", s1+1);
        s3 = gpsstr.indexOf(",", s2+1);
        s4 = gpsstr.indexOf(",", s3+1);
        s5 = gpsstr.indexOf(",", s4+1);

        // Lat, Long 위치의 값 index로 추출
        Lat = gpsstr.substring(s2+1, s3); //위도 ddmm.mmmm
        Long = gpsstr.substring(s4+1, s5); // 경도 dddmm.mmmm

        // Lat 앞,뒤의 값 구분
        Lat1 = Lat.substring(0, 2);
        Lat2 = Lat.substring(2);
        // Long 앞,뒤의 값 구분
        Long1 = Long.substring(0, 3);
        Long2 = Long.substring(3);

        // 좌표 계산
        Lat_ = Lat1.toDouble() + Lat2.toDouble()/60;
        Long_ = Long1.toFloat() + Long2.toFloat()/60;
        // 좌표 출력
        Serial.print("Lat:");
        Serial.println(Lat_, 15);
        Serial.print("Long:");
        Serial.println(Long_, 15); // 소수점 15번째 자리까지 출력
      }
// GPRMC
      if(targetStr1.equals(gpsstr.substring(1,6))) 
      // 지금까지 저장된 str값이 targetStr 값과 동일한지 확인
      {
        Serial.println(gpsstr);
        // , : 토큰
        s1 = gpsstr.indexOf(",");
        s2 = gpsstr.indexOf(",", s1+1);
        s3 = gpsstr.indexOf(",", s2+1);
        s4 = gpsstr.indexOf(",", s3+1);
        s5 = gpsstr.indexOf(",", s4+1);
        s6 = gpsstr.indexOf(",", s5+1);
        s7 = gpsstr.indexOf(",", s6+1);
        s8 = gpsstr.indexOf(",", s7+1);
        s9 = gpsstr.indexOf(",", s8+1);
        s10 = gpsstr.indexOf(",", s9+1);

        // Date, Time 위치의 값 index로 추출
        Date = gpsstr.substring(s9+1, s10); //날짜 DDMMYY
        Time = gpsstr.substring(s1+1, s2);  //시간 hhmmss.sss

        // Date의 부분 값 index로 추출
        Year = Date.substring(4,6);
        Mon = Date.substring(2,4); 
        Day = Date.substring(0,2);

        // Time의 부분 값 index로 추출
        Hour = Time.substring(0,2);
        Min = Time.substring(2,4);
        Sec = Time.substring(4,6);

        // 값 계산
        Day_ = Day.toInt() + 1;
        Hour_ = Hour.toInt() + 9;
        if (Hour_ > 24)
        {
          Hour_ -= 24;
        }
        Sec_ = Sec.toInt() + 8;
        if (Sec_ >= 60)
        {
          Min_ += 1;
          Sec_ = 0;
        }

        // 출력
        Serial.print("Date:");
        Serial.print(Year);
        Serial.print("-");
        Serial.print(Mon);
        Serial.print("-");
        Serial.println(Day_);

        Serial.print("Time:");
        Serial.print(Hour_);
        Serial.print(":");
        Serial.print(Min);
        Serial.print(":");
        Serial.println(Sec_);
      }
      // gpsstr 값 초기화
      gpsstr = "";
    }
    else // \n이 아닐 시
    {
      gpsstr += data; // gpsstr에 문자 계속 더함
    }
  }
}