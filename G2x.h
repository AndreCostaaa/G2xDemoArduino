#ifndef G2X_H
#define G2X_H

#include "Arduino.h"
#define LOGITECH_G27


#ifdef LOGITECH_G27
  #define WHEEL_AXIS 0
  #define THROTTLE_AXIS 2
  #define BRAKE_AXIS 3
  #define CLUTCH_AXIS 1
#elif defined(LOGITECH_G29)

  #define WHEEL_AXIS 0
  #define THROTTLE_AXIS 2
  #define BRAKE_AXIS 3
  #define CLUTCH_AXIS 1
#endif

  #define MAX_BUFFER_SIZE 50
  #define NB_AXIS 10
  #define NB_BUTTONS 50
  #define NB_HATS 10
  typedef enum
  {
    SET = 'S',
    GET = 'G'
  }CommandEnum;
  
  typedef enum
  {
    AXIS = 'A',
    BUTTON = 'B',
    HAT = 'H',
    MODE = 'M',
    NUMBER = 'N'
  }CommandDetailEnum;
  
  typedef enum
  {
    ON_REQUEST = 'R',
    AUTO = 'O'
  }ModeEnum;



  class LOGITECH_G2x
  {
    public:
      LOGITECH_G2x() = default;
      void begin(HardwareSerial* serial, uint32_t timeout);
      void begin(HardwareSerial* serial);
      /* 
      getName(char* name);
      void getGUID(char* guid);
      */
      bool getWheel(uint8_t *val);
      bool getThrottle(uint8_t *val);
      bool getBrake(uint8_t *val);
      bool getClutch(uint8_t *val);
      bool getHat(uint32_t hatNumber, String *val);
      bool getButton(uint32_t btnNumber, bool* val);
    private:
      char m_OutputBuffer[MAX_BUFFER_SIZE];
      char m_InputBuffer[MAX_BUFFER_SIZE];
      uint8_t m_AxisValuesArr[NB_AXIS];
      bool m_ButtonValuesArr[NB_BUTTONS];
      String m_HatValuesArr[NB_HATS];
      uint32_t m_Timeout;
      int32_t m_InputBufferSize;
      int32_t m_NumberOfButtons;
      int32_t m_NumberOfHats;
      int32_t m_NumberOfAxes;
      
      HardwareSerial* m_Serial;
    private:
      void beginCommon();
      bool readAxis(uint32_t axis);
      bool readButton(uint32_t btn);
      bool readHat(uint32_t hat);
      void gendRequest(const char* request);
      bool gotResponse();
      uint8_t readResponse();
      void createOutputBuffer(CommandEnum cmd, CommandDetailEnum cmdDetail, uint8_t value);
      void request(CommandDetailEnum cmdDetail, uint8_t value);
      bool waitForResponse();
      String readFromInputBuffer();
      void sendOutputBuffer();
      static int32_t findChar(const char charToFind, const char* buf, int32_t len);
      size_t getNumberOf(CommandDetailEnum thing);
  };

#endif
