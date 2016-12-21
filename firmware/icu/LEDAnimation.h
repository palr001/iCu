
#define STATE_IDLE 0
#define STATE_RUNNING 1

class LEDAnimation {
  public:
    uint8_t r, g, b;
    float runDuration = 0;
    float timeout = 5.0;
    uint8_t state = STATE_IDLE;

    virtual void begin(uint8_t r, uint8_t g, uint8_t b, float param1, float param2, float param3) {
    }

    void begin(uint32_t color, float param1, float param2, float param3){
      begin((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, param1, param2, param3);
    }
    
    void begin(LEDAnimation other, float param1 = 0, float param2 = 0, float param3 = 0){
      begin(other.r, other.g, other.b, param1, param2, param3);  
    }
      
    virtual void animationUpdate(float dt) {
    }

    void update(float dt) {
      if (state == STATE_RUNNING) {
        animationUpdate(dt);
        runDuration += dt;
      }

      if (runDuration > timeout) {
        state = STATE_IDLE;
      }

      if (state == STATE_IDLE) {        
        runDuration = 0;
      }
    }

    void setTimeout(float timeout) {
      this->timeout = timeout;
    }

    void stop() {
      state = STATE_IDLE;
    }

};

class SpringAnimation : public LEDAnimation {

  public:
    SpringyValue springyValue;

    void begin(uint8_t r, uint8_t g, uint8_t b, float offset, float k, float c) {
      this->r = r;
      this->g = g;
      this->b = b;
      
      state = STATE_RUNNING;

      springyValue.reset();

      springyValue.o = offset;
      springyValue.k = k;
      springyValue.c = c;

    }

    void animationUpdate(float dt) {
      //Serial.println(springyValue.x);
      springyValue.update(dt);
      setAllPixels(r, g, b, abs(springyValue.x));
    }

};

class BreatheAnimation  : public LEDAnimation {
  public:
    float x = 0, v = 0, vInhale = 0.0, vExhale = 0.0, interval = 0.0;

    void begin(uint8_t r, uint8_t g, uint8_t b, float vInhale, float vExhale, float interval) {
      state = STATE_RUNNING;
      
      this->r = r;
      this->g = g;
      this->b = b;

      v = vInhale;
      this->vInhale = vInhale;
      this->vExhale = vExhale;
      this->interval = interval;      
    }

    void animationUpdate(float dt) {
      x += v / dt;
      if (x > 255.0) v = -vExhale;
      if (x < 0) v = vInhale;
      x = constrain(x, 0, 255);
      setAllPixels(r, g, b, x);
    }

};


