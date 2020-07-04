#include <EEPROM.h>

int exterR = 9;
int exterG = 11;
int exterB = 10;
int interR = 5;
int interG = 3;
int interB = 6;
int red    = 0;
int green  = 0;
int blue   = 0;
int Fred   = 0;
int Fgreen = 0;
int Fblue  = 0;
int brightness = 0;
int wait   = 0;
int flashColor = 0;

unsigned long int flashTime = 0;
unsigned long int Lasttime  = 0;
unsigned long int ITime = 0;
unsigned long int ETime = 0;
unsigned long int showTime  = 0;
int spd   = 0;
int timer = 0;

unsigned long appTimer = 0;
bool app = false;

char ind1 = "";
char ind2 = "";
char ind3 = "";
char ind4 = "";
char ind5 = "";
char ind6 = "";
char ind7 = "";

int status     = 0;
int cfgbghtnss = 0;
int animate    = 0;
int cfgred     = 0;
int cfggreen   = 0;
int cfgblue    = 0;
int cfgspeed   = 0;
int countAnim  = 0;

struct ledsConfig
{
    int eeprom;
    int status;
    int brightness;
    int animate;
    byte red;
    byte green;
    byte blue;
    int speed;
};

struct appConfig
{
    int status;
    int brightness;
    int animate;
    byte red;
    byte green;
    byte blue;
    int speed;
};

struct ledsConfig cfgLeds;
struct appConfig cfgApp;

void setup()
{
    Serial.begin(9600);
    Serial.println("{\"arduino\":true}");
    pinMode(interR, OUTPUT);
    pinMode(interG, OUTPUT);
    pinMode(interB, OUTPUT);
    pinMode(exterR, OUTPUT);
    pinMode(exterG, OUTPUT);
    pinMode(exterB, OUTPUT);

    cfgLeds.eeprom = 0;
    cfgApp = {0, 0, 1, 0, 0, 0, 100};

    if(EEPROM[0] != 1)
    {
        EEPROM.write(0, 1);
        EEPROM.write(1, 1);
        EEPROM.write(2, 100);
        EEPROM.write(4, 1);
        EEPROM.write(6, 255);
        EEPROM.write(7, 255);
        EEPROM.write(8, 255);
        EEPROM.write(9, 10);

        cfgLeds = {0, 1, 100, 1, 255, 0, 0, 100};
    }
}

void loop()
{
    // String iindex = "testando o novoo metodo";
    // char iindexCopy[50];
    // iindex.toCharArray(iindexCopy, 50);


    // Serial.println(iindexCopy);

    if(cfgLeds.eeprom != 1)
    {
        cfgLeds.eeprom     = 1;
        cfgLeds.status     = EEPROM[1];
        cfgLeds.brightness = EEPROM[2];
        cfgLeds.animate    = EEPROM[4];
        cfgLeds.red        = EEPROM[6];
        cfgLeds.green      = EEPROM[7];
        cfgLeds.blue       = EEPROM[8];
        cfgLeds.speed      = EEPROM[9];
    }

    unsigned long currentMillis = millis();

    if(app && (currentMillis - appTimer > 20000))
    {
        appTimer = currentMillis;
        app      = false;
        cfgApp   = {0, 0, 1, 0, 0, 0, 100};
    }

    SerialUpdate();
}

void SerialUpdate()
{
    if(Serial.available() > 0)
    {
        unsigned long currentMillis = millis();
        String serialRead = Serial.readString();
        String index = serialRead.substring(0, serialRead.indexOf('='));
        String value = serialRead.substring(serialRead.indexOf('=') + 1);

        // if(serialRead.indexOf(',', serialRead.indexOf('=')))
        // Serial.write(serialRead.indexOf(',', serialRead.indexOf('=')));
        // char indexCopy[50];
        // index.toCharArray(indexCopy, 50);

        if(index == "app" && value == "true")
        {
            app = true;
            appTimer = currentMillis;
        }
        else if(index == "app" && value == "false")
            app = false;
        else if(index == "data")
        {
            ind1          = value.indexOf(';');
            String index2 = value.substring(0, ind1);

            ind2       = value.indexOf(';', ind1+1);
            status     = value.substring(ind1+1, ind2+1).toInt();

            ind3       = value.indexOf(';', ind2+1);
            cfgbghtnss = value.substring(ind2+1, ind3+1).toInt();

            ind4       = value.indexOf(';', ind3+1);
            animate    = value.substring(ind3+1, ind4+1).toInt();

            ind5       = value.indexOf(';', ind4+1);
            cfgred     = value.substring(ind4+1, ind5+1).toInt();

            ind6       = value.indexOf(';', ind5+1);
            cfggreen   = value.substring(ind5+1, ind6+1).toInt();

            ind7       = value.indexOf(';', ind6+1);
            cfgblue    = value.substring(ind6+1).toInt();

            cfgspeed   = value.substring(ind7+1).toInt();

            if(index2 == "true")
            {
                EEPROM.write(1, status);
                EEPROM.write(2, cfgbghtnss);
                EEPROM.write(4, animate);
                EEPROM.write(6, cfgred);
                EEPROM.write(7, cfggreen);
                EEPROM.write(8, cfgblue);
                EEPROM.write(9, cfgspeed);
            }

            if(app)
            {
                cfgApp.status     = status;
                cfgApp.brightness = cfgbghtnss;
                cfgApp.animate    = animate;
                cfgApp.red        = cfgred;
                cfgApp.green      = cfggreen;
                cfgApp.blue       = cfgblue;
                cfgApp.speed      = cfgspeed;
            }
            else
            {
                cfgLeds.status     = status;
                cfgLeds.brightness = cfgbghtnss;
                cfgLeds.animate    = animate;
                cfgLeds.red        = cfgred;
                cfgLeds.green      = cfggreen;
                cfgLeds.blue       = cfgblue;
                cfgLeds.speed      = cfgspeed;
            }

            flashColor = 0;
            countAnim = 0;
            wait = 0;
        }
    }

    if(app)
        ConfigLeds(cfgApp.status, cfgApp.brightness, cfgApp.animate, cfgApp.red, cfgApp.green, cfgApp.blue, cfgApp.speed);
    else
        ConfigLeds(cfgLeds.status, cfgLeds.brightness, cfgLeds.animate, cfgLeds.red, cfgLeds.green, cfgLeds.blue, cfgLeds.speed);
}

void ConfigLeds(int onoff, int brightness, int animate, int r, int g, int b, int spd)
{
    //animate:
    // 1: solid
    // 2: pulse
    // 3: chroma
    // 4: Team

    if(onoff == 0)
        brightness = 0;

    unsigned long currentMillis = millis();

    timer = 100 - spd;

    if(currentMillis - flashTime > timer)
    {
        flashTime = currentMillis;

        if(animate == 1)
        {
            red   = r;
            green = g;
            blue  = b;
        }
        else if(animate == 2)
        {
            int maxwait = 1000;
            int minwait = 150;
                
            if(flashColor < 100 && wait <= 0)
                flashColor += 1;
            else if(flashColor >= 100 && wait < maxwait)
                wait += 1;
            else if(flashColor > 0 && wait >= maxwait)
                flashColor -= 1;
            else if(flashColor <= 0 && wait > (maxwait - minwait))
                wait -= 1;
            else if(flashColor <= 0 && wait == (maxwait - minwait))
                wait = 0;

            red   = map(flashColor, 0, 100, 0, r);
            green = map(flashColor, 0, 100, 0, g);
            blue  = map(flashColor, 0, 100, 0, b);
        }
        else if(animate == 3)
        {
            if(red >= 255 && green >= 255 && blue >= 255)
            {
                red = 0;
                green = 0;
                blue = 0;
            }
            else if(red <= 0 && green <= 0 && blue <= 0)
                red = 255;
            else if(red >= 255 && green < 255 && blue <= 0)
                green += 1;
            else if(green >= 255 && red > 0 && blue <= 0)
                red -= 1;
            else if(green >= 255 && red <= 0 && blue < 255)
                blue += 1;
            else if(blue >= 255 && green > 0 && red <= 0)
                green -= 1;
            else if(blue >= 255 && green <= 0 && red < 255)
                red += 1;
            else if(red <= 255 && blue > 0 && green <= 0)
                blue -= 1;
        }
        else if(animate == 4)
        {
            int maxwait = 10;

            if(flashColor < 100 && wait <= 0)
                flashColor += 1;
            else if(flashColor >= 100 && wait < maxwait)
                wait += 1;
            else if(flashColor > 0 && wait >= maxwait)
                flashColor -= 1;
            else if(flashColor <= 0 && wait > 0)
                wait -= 1;
            
            if(flashColor >= 100 && wait <= 0)
                countAnim++;

            if(countAnim >= 4)
            {
                red   = r;
                green = g;
                blue  = b;
            }
            else
            {
                red   = map(flashColor, 0, 100, 0, r);
                green = map(flashColor, 0, 100, 0, g);
                blue  = map(flashColor, 0, 100, 0, b);
            }
        }
        else if(animate == 5)
        {
            int maxwait = 1500;

            if(flashColor <= 0 && wait < maxwait)
                wait += 1;
            
            if(flashColor < 100 && wait >= maxwait && countAnim < 2)
                flashColor += 1;
            else if(flashColor > 0 && wait >= maxwait && countAnim > 2)
                flashColor -= 1;

            if(flashColor >= 100 && wait >= maxwait && countAnim == 0)
                countAnim = 3;
            else if(flashColor <= 0 && wait >= maxwait && countAnim == 3)
                countAnim = 1;
            if(flashColor >= 100 && wait >= maxwait && countAnim == 1)
                countAnim = 4;
            else if(flashColor <= 0 && wait >= maxwait && countAnim == 4)
            {
                countAnim = 0;
                wait = 0;
            }

            red   = map(flashColor, 0, 100, 0, r);
            green = map(flashColor, 0, 100, 0, g);
            blue  = map(flashColor, 0, 100, 0, b);
        }
        else if(animate == 6)
        {
            if(countAnim == 0 && flashColor == 0)
            {
                countAnim = 1;
                flashColor = 100;
            }
            else if(countAnim >= 1 && flashColor > 0)
                flashColor -= 1;

            red   = map(flashColor, 0, 100, 0, r);
            green = map(flashColor, 0, 100, 0, g);
            blue  = map(flashColor, 0, 100, 0, b);
        }

        Fred   = map(brightness, 0, 100, 0, red);
        Fgreen = map(brightness, 0, 100, 0, green);
        Fblue  = map(brightness, 0, 100, 0, blue);
    
        analogWrite(exterR, Fred);
        analogWrite(exterG, Fgreen);
        analogWrite(exterB, Fblue);

        analogWrite(interR, Fred);
        analogWrite(interG, Fgreen);
        analogWrite(interB, Fblue);
    }
}
