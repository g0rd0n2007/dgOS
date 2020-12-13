class Timer{
private:
  boolean _Enabled;
public:
  uint32_t TargetTime;
  uint32_t Duration;
  boolean Enabled;

  Timer(){
    Duration = 1000;
    TargetTime = 0;
    Enabled = false;
  }

  void Set(uint32_t now, uint32_t duration){
    Duration = duration;
    TargetTime = now + duration;
    Enabled = true;
  }

  void SetNext(uint32_t now){
    TargetTime = now + Duration;
    Enabled = true;
  }

  void SetEnabled(uint32_t now, boolean enabled){
    _Enabled = Enabled;
    Enabled = enabled;
    if(Enabled != _Enabled && enabled){//switching on after pause
      Set(now, Duration);
    }
  }

  boolean TON(uint32_t now, boolean update_target_time = false){
    if(TargetTime < now && Enabled){
      if(update_target_time) TargetTime = now + Duration;
      return true;
    }else return false;    
  }
  
  
};
