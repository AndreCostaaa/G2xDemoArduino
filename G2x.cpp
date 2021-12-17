#include "G2x.h"


#define START_TOKEN ':'
#define END_TOKEN '\n'
#define REQUEST_SIZE 5
#define DEFAULT_TIMEOUT 250
#define START_INDEX_VALUE 4

void LOGITECH_G2x::begin(HardwareSerial* serial)
{
  m_Serial = serial;
  m_Timeout = DEFAULT_TIMEOUT;
  beginCommon();
}
void LOGITECH_G2x::begin(HardwareSerial* serial, uint32_t timeout)
{
  m_Serial = serial;
  m_Timeout = timeout;
  beginCommon();
}

void LOGITECH_G2x::beginCommon()
{
  m_Serial->begin(115200);
  while (!m_Serial);
  m_Serial->setTimeout(m_Timeout);

  m_NumberOfButtons = getNumberOf(BUTTON);
  m_NumberOfHats = getNumberOf(HAT);
  m_NumberOfAxes = getNumberOf(AXIS);
}

bool LOGITECH_G2x::getButton(uint32_t btnNumber, bool *val)
{
  if (btnNumber < 0 || btnNumber >= m_NumberOfButtons)
  {
    return false;
  }
  bool ret = readButton(btnNumber);

  *val = m_ButtonValuesArr[btnNumber];
  return ret;

}

bool LOGITECH_G2x::getHat(uint32_t hatNumber, String *val)
{
  if (hatNumber < 0 || hatNumber >= m_NumberOfHats)
  {
    return false;
  }
  bool ret = readHat(hatNumber);

  *val = m_HatValuesArr[hatNumber];
  return ret;
}

bool LOGITECH_G2x::getWheel(uint8_t *val)
{
#ifdef WHEEL_AXIS
  bool ret = readAxis(WHEEL_AXIS);

  *val = m_AxisValuesArr[WHEEL_AXIS];

  return ret;
#else
  return false;
#endif
}

bool LOGITECH_G2x::getThrottle(uint8_t *val)
{
#ifdef THROTTLE_AXIS
  bool ret = readAxis(THROTTLE_AXIS);

  *val = m_AxisValuesArr[THROTTLE_AXIS];
  return ret;
#else
  return false;
#endif
}
bool LOGITECH_G2x::getBrake(uint8_t *val)
{
#ifdef BRAKE_AXIS
  bool ret = readAxis(BRAKE_AXIS);

  *val = m_AxisValuesArr[BRAKE_AXIS];
  return ret;
#else
  return false;
#endif
}
bool LOGITECH_G2x::getClutch(uint8_t *val)
{
#ifdef BRAKE_AXIS
  bool ret = readAxis(CLUTCH_AXIS);

  *val = m_AxisValuesArr[CLUTCH_AXIS];
  return ret;
#else
  return false;
#endif
}
void LOGITECH_G2x::request( CommandDetailEnum cmdDetail, uint8_t value)
{
  uint8_t aux[] =
  {
    START_TOKEN,
    GET,
    cmdDetail,
    (uint8_t)value / 10 + '0',
    (uint8_t)value % 10 + '0'
  };

  for (int i = 0; i < REQUEST_SIZE; i ++)
  {
    m_OutputBuffer[i] = aux[i];
  }

  sendOutputBuffer();
}
void LOGITECH_G2x::sendOutputBuffer()
{
  m_Serial->println(m_OutputBuffer);
}
bool LOGITECH_G2x::waitForResponse()
{
  m_InputBufferSize = m_Serial->readBytesUntil(END_TOKEN, m_InputBuffer, MAX_BUFFER_SIZE);
  return m_InputBufferSize > 0;
}

size_t LOGITECH_G2x::getNumberOf(CommandDetailEnum thing)
{
  {
    uint8_t aux[] =
    {
      START_TOKEN,
      GET,
      NUMBER,
      thing
    };
    const int len = 4;
    for (int i = 0; i < len; i ++)
    {
      m_OutputBuffer[i] = aux[i];
    }
  }

  sendOutputBuffer();

  if (waitForResponse())
  {
    return readFromInputBuffer().toInt();
  }
  return 0;
}
bool LOGITECH_G2x::readButton(uint32_t btn)
{
  request(BUTTON, btn);
  if (waitForResponse())
  {
    m_ButtonValuesArr[btn] = (bool)readFromInputBuffer().toInt();
    return true;
  }
  return false;
}

bool LOGITECH_G2x::readAxis(uint32_t axis)
{
  request(AXIS, axis);
  if (waitForResponse())
  {
    m_AxisValuesArr[axis] = (uint8_t)(readFromInputBuffer().toInt());

    return true;
  }
  return false;
}

bool LOGITECH_G2x::readHat(uint32_t hat)
{
  request(HAT, hat);
  if (waitForResponse())
  {
    m_HatValuesArr[hat] = readFromInputBuffer();
    return true;
  }
  return false;
}

String LOGITECH_G2x::readFromInputBuffer()
{
  m_InputBuffer[m_InputBufferSize] = 0;
  String valStr(m_InputBuffer + START_INDEX_VALUE);
  return valStr;
}
