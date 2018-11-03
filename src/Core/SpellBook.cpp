#include "SpellBook.h"
#include "InitManager.h"
#include "Utils/RobotDefs.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Utils/Math.h"

using namespace std;
namespace po = boost::program_options;

SpellBook::SpellBook()
{
    pthread_mutex_init(&lock, NULL);
}

SpellBook::~SpellBook()
{
    pthread_mutex_destroy(&lock);
}

void SpellBook::Load(string fileName)
{
    Storage storage(fileName);
    modules.Load(storage);
    motion.Load(storage);
    perception.Load(storage);
    remote.Load(storage);
    strategy.Load(storage);
    behaviour.Load(storage);
    network.Load(storage);
}

void SpellBook::Save(string fileName)
{
    Storage storage(fileName);
    modules.Save(storage);
    motion.Save(storage);
    perception.Save(storage);
    remote.Save(storage);
    strategy.Save(storage);
    behaviour.Save(storage);
    network.Save(storage);

    storage.Save();
}

void SpellBook::AddOptions(po::options_description &description)
{
    modules.AddOptions(description);
    motion.AddOptions(description);
    perception.AddOptions(description);
    remote.AddOptions(description);
    strategy.AddOptions(description);
    behaviour.AddOptions(description);    
    network.AddOptions(description);
}

void SpellBook::Update()
{
    po::variables_map config = InitManager::GetBlackboard()->config;
    modules.Update(config);
    motion.Update(config);
    perception.Update(config);
    remote.Update(config);
    strategy.Update(config);
    behaviour.Update(config);
    network.Update(config);
}

void SpellBook::Lock()
{
    pthread_mutex_lock(&lock);
}

void SpellBook::Unlock()
{
    pthread_mutex_unlock(&lock);
}

// Spell

Spell::Spell()
{

}

Spell::~Spell()
{

}

void Spell::CopyTo(Spell *spell)
{

}

void Spell::Load(Storage &storage)
{

}

void Spell::Save(Storage &storage)
{

}

void Spell::AddOptions(po::options_description &description)
{

}

void Spell::Update(const po::variables_map &config)
{

}

// Modules
ModulesSpell::ModulesSpell()
{
    LoadStrategy = true;
    LoadMotion = true;
    LoadPerception = true;
    LoadRemote = true;
    LoadBehaviour = true;
    LoadNetwork = true;
}

void ModulesSpell::CopyTo(Spell *spell)
{

}

void ModulesSpell::Load(Storage &storage)
{
    LoadStrategy = storage["Modules"]["Strategy"]["Enabled"].Default(true);
    LoadMotion = storage["Modules"]["Motion"]["Enabled"].Default(true);
    LoadPerception = storage["Modules"]["Perception"]["Enabled"].Default(true);
    LoadRemote = storage["Modules"]["Remote"]["Enabled"].Default(true);
    LoadBehaviour = storage["Modules"]["Behaviour"]["Enabled"].Default(true);
    LoadNetwork = storage["Modules"]["Network"]["Enabled"].Default(true);
}

void ModulesSpell::Save(Storage &storage)
{
    storage["Modules"]["Strategy"]["Enabled"] = LoadStrategy;
    storage["Modules"]["Motion"]["Enabled"] = LoadMotion;
    storage["Modules"]["Perception"]["Enabled"] = LoadPerception;
    storage["Modules"]["Remote"]["Enabled"] = LoadRemote;
    storage["Modules"]["Behaviour"]["Enabled"] = LoadBehaviour;
    storage["Modules"]["Network"]["Enabled"] = LoadNetwork;
}

BallSpell::BallSpell()
{
    Enabled = true;
    method = "CASCADE";
    ballWidth = ballHeight = 0.1f;

    BallYaw = 0;
    BallDetected = false;
    BallDistance = 0;
    BallPitch = 0;
    ImageX = ImageY = 0;
    BallLostCount = 0;
    HeadYaw = HeadPitch = 0;
}

void BallSpell::CopyTo(Spell *spell)
{
    BallSpell *s = (BallSpell*)spell;
    COPY(s, Enabled)
    COPY(s, method)
    COPY(s, ballWidth)
    COPY(s, ballHeight)
    COPY(s, BallYaw)
    COPY(s, HeadSpeed)
    COPY(s, BallDetected)
    COPY(s, BallDistance)
    COPY(s, BallPitch)
    COPY(s, ImageX)
    COPY(s, ImageY)
    COPY(s, BallLostCount)
    COPY(s, HeadYaw)
    COPY(s, HeadPitch)
}

void BallSpell::Load(Storage &storage)
{
    Enabled = storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Enabled"].Default(true);
    method = (string)storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Method"].Default("CASCADE");
    ballWidth = storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Ball"]["Size"][0].Default(0.1f);
    ballHeight = storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Ball"]["Size"][1].Default(0.1f);
}

void BallSpell::Save(Storage &storage)
{
    storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Enabled"] = Enabled;
    storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Method"] = method;
    storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Ball"]["Size"][0] = ballWidth;
    storage["Modules"]["Perception"]["Vision"]["BallDetector"]["Ball"]["Size"][1] = ballHeight;
}

LocalizationSpell::LocalizationSpell()
{
    Enabled = true;

    X = -2;
    Y = -3;
    Theta = 0;
}

void LocalizationSpell::CopyTo(Spell *spell)
{
    LocalizationSpell *s = (LocalizationSpell*)spell;
    COPY(s, Enabled)
    COPY(s, X)
    COPY(s, Y)
    COPY(s, Theta)
}

void LocalizationSpell::Load(Storage &storage)
{
    Enabled = storage["Modules"]["Perception"]["Vision"]["Localization"]["Enabled"].Default(true);
    
    X = storage["Modules"]["Perception"]["Vision"]["Localization"]["Position"][0].Default(0.0f);
    Y = storage["Modules"]["Perception"]["Vision"]["Localization"]["Position"][1].Default(0.0f);
    Theta = storage["Modules"]["Perception"]["Vision"]["Localization"]["Position"][2].Default(0.0f);
}

void LocalizationSpell::Save(Storage &storage)
{
    storage["Modules"]["Perception"]["Vision"]["Localization"]["Enabled"] = Enabled;
}

VisionSpell::VisionSpell()
{
    Enabled = true;
    Record = false;
    VideoName = "video.avi";
    BGR = false;
    HSV = false;
    GRAY = false;  
}

void VisionSpell::CopyTo(Spell *spell)
{
    VisionSpell *s = (VisionSpell*)spell;
    ball.CopyTo(&(s->ball));
    localization.CopyTo(&(s->localization));
    COPY(s, Enabled)
    COPY(s, Record)
    COPY(s, VideoName)
    COPY(s, BGR)
    COPY(s, HSV)
    COPY(s, GRAY)
}

void VisionSpell::Load(Storage &storage)
{
    Enabled = storage["Modules"]["Perception"]["Vision"]["Enabled"].Default(true);
    Record = storage["Modules"]["Perception"]["Vision"]["Record"].Default(false);
    VideoName = (string)storage["Modules"]["Perception"]["Vision"]["VideoName"].Default("video.avi");
    ball.Load(storage);
    localization.Load(storage);
}

void VisionSpell::Save(Storage &storage)
{
    ball.Save(storage);
    localization.Save(storage);
    storage["Modules"]["Perception"]["Vision"]["Enabled"] = Enabled;
}

PerceptionSpell::PerceptionSpell()
{
    
}

void PerceptionSpell::CopyTo(Spell *spell)
{
    PerceptionSpell *s = (PerceptionSpell*)spell;
    vision.CopyTo(&(s->vision));
}

void PerceptionSpell::Load(Storage &storage)
{
    vision.Load(storage);
}

void PerceptionSpell::Save(Storage &storage)
{
    vision.Save(storage);
}

MotionSpell::MotionSpell()
{
    Remote = false;
    Stand = Stiff = false;
    Vx = Vy = Vth = 0;
    HeadYaw = HeadPitch = 0;
    HeadSpeedYaw = HeadSpeedPitch = 0.2f;
    HeadRelative = false;
    KickLeft = KickRight = false;
    LimpLeft = LimpRight = false;
    GetupBack = GetupFront = false;
    TipOver = Dead = false;
    Walk = false;
    Crouch = false;

    GoalieSit = false;
    GoalieCentre = false;
    GoalieUncentre = false;
    GoalieStand = false;
    GoalieInitial = false;
    DefenderCentre = false;
    ThrowIn = false;

    Calibrate = false;
    AngleX = AngleY = GyroX = GyroY = false;
}

void MotionSpell::CopyTo(Spell *spell)
{
    MotionSpell *s = (MotionSpell*)spell;
    COPY(s, Remote)
    COPY(s, Stand)
    COPY(s, Stiff)
    COPY(s, Vx)
    COPY(s, Vy)
    COPY(s, Vth)
    COPY(s, HeadYaw)
    COPY(s, HeadPitch)
    COPY(s, HeadSpeedYaw)
    COPY(s, HeadSpeedPitch)
    COPY(s, HeadRelative)
    COPY(s, KickLeft)
    COPY(s, KickRight)
    COPY(s, LimpLeft)
    COPY(s, LimpRight)
    COPY(s, GetupBack)
    COPY(s, GetupFront)
    COPY(s, TipOver)
    COPY(s, Dead)
    COPY(s, Walk)
    COPY(s, Crouch)

    COPY(s, GoalieSit)
    COPY(s, GoalieCentre)
    COPY(s, GoalieUncentre)
    COPY(s, GoalieStand)
    COPY(s, GoalieInitial)
    COPY(s, DefenderCentre)
    COPY(s, ThrowIn)

    COPY(s, Calibrate)
    COPY(s, AngleX)
    COPY(s, AngleY)
    COPY(s, GyroX)
    COPY(s, GyroY)
}

void MotionSpell::AddOptions(po::options_description &description)
{
    description.add_options()
            ("motion.calibrate", po::value<bool>()->default_value(false), "Calibrates the robot");
}

void MotionSpell::Update(const po::variables_map &config)
{
    Calibrate = config["motion.calibrate"].as<bool>();
}

void MotionSpell::Load(Storage &storage)
{
    
}

void MotionSpell::Save(Storage &storage)
{
    
}

RemoteSpell::RemoteSpell()
{
    EnableJoystick = true;
    EnableNetwork = true;
}

void RemoteSpell::CopyTo(Spell *spell)
{
    RemoteSpell *s = (RemoteSpell*)spell;
    COPY(s, EnableJoystick)
    COPY(s, EnableNetwork)
}

void RemoteSpell::Load(Storage &storage)
{
    EnableJoystick = storage["Modules"]["Remote"]["Joystick"]["Enabled"].Default(true);
    EnableNetwork = storage["Modules"]["Remote"]["Network"]["Enabled"].Default(true);
}

void RemoteSpell::Save(Storage &storage)
{
    storage["Modules"]["Remote"]["Joystick"]["Enabled"] = EnableJoystick;
    storage["Modules"]["Remote"]["Network"]["Enabled"] = EnableNetwork;
}

NetworkSpell::NetworkSpell()
{
    TCPConnected = false;
    SelectedCamera = 0;
    topSettings.brightnessChanged = botSettings.brightnessChanged = false;
    topSettings.saturationChanged = botSettings.saturationChanged = false;
    topSettings.contrastChanged = botSettings.contrastChanged = false;
    topSettings.sharpnessChanged = botSettings.sharpnessChanged = false;
}

void NetworkSpell::CopyTo(Spell *spell)
{
    NetworkSpell *s = (NetworkSpell*)spell;
    COPY(s, TCPConnected)
    COPY(s, SelectedCamera)


    COPY(s, topSettings.brightnessChanged)
    COPY(s, topSettings.saturationChanged)
    COPY(s, topSettings.contrastChanged)
    COPY(s, topSettings.sharpnessChanged)
    COPY(s, topSettings.brightness)
    COPY(s, topSettings.saturation)
    COPY(s, topSettings.contrast)
    COPY(s, topSettings.sharpness)

    COPY(s, botSettings.brightnessChanged)
    COPY(s, botSettings.saturationChanged)
    COPY(s, botSettings.contrastChanged)
    COPY(s, botSettings.sharpnessChanged)
    COPY(s, botSettings.brightness)
    COPY(s, botSettings.saturation)
    COPY(s, botSettings.contrast)
    COPY(s, botSettings.sharpness)
}

void NetworkSpell::Load(Storage &storage)
{
    
}

void NetworkSpell::Save(Storage &storage)
{
    
}

StrategySpell::StrategySpell()
{
    GameState = GC::INITIAL;
    WalkInCircle = WalkInSquare = false;
    Started = false;
    Penalized = false;
    FallenBack = false;
    FallenFront = false;
    Die = false;
    TurnOver = false;
    WalkAside = false;
    WalkForward = false;
    TargetX = 0;
    TargetY = 0;
    TargetTheta = 0;

    HeadYawRange = Deg2Rad(50.0f);
    HeadPitchRange = Deg2Rad(20.0f);
    HeadSearchSpeed = 0.1f;
    HeadScanCount = 0;
}

void StrategySpell::CopyTo(Spell *spell)
{
    StrategySpell *s = (StrategySpell*)spell;
    COPY(s, GameState)
    COPY(s, WalkInCircle)
    COPY(s, WalkInSquare)
    COPY(s, Started)
    COPY(s, Penalized)
    COPY(s, FallenBack)
    COPY(s, FallenFront)
    COPY(s, Die)
    COPY(s, TurnOver)
    COPY(s, WalkAside)
    COPY(s, WalkForward)
    COPY(s, TargetX)
    COPY(s, TargetY)
    COPY(s, TargetTheta)

    COPY(s, HeadYawRange)
    COPY(s, HeadPitchRange)
    COPY(s, HeadSearchSpeed)
    COPY(s, HeadScanCount)
}

void StrategySpell::Load(Storage &storage)
{
    
}

void StrategySpell::Save(Storage &storage)
{
    
}

void StrategySpell::AddOptions(po::options_description &description)
{
    description.add_options()
            ("walk.circle", po::value<bool>()->default_value(false), "walks in circle")
            ("walk.square", po::value<bool>()->default_value(false), "walks in square");
}

void StrategySpell::Update(const po::variables_map &config)
{
    WalkInCircle = config["walk.circle"].as<bool>();
    WalkInSquare = config["walk.square"].as<bool>();
}

BehaviourSpell::BehaviourSpell()
{
    Started = false;
    Penalized = false;
    Fallen = false;
    Die = false;
    LeftEye = WHITE;
    RightEye = WHITE;
    Number = 2;
    Name = "NAO";
}

void BehaviourSpell::CopyTo(Spell *spell)
{
    BehaviourSpell *s = (BehaviourSpell*)spell;
    COPY(s, Started)
    COPY(s, Penalized)
    COPY(s, Fallen)
    COPY(s, Die)
    COPY(s, LeftEye)
    COPY(s, RightEye)
    COPY(s, Number)
    COPY(s, Name)
}

void BehaviourSpell::Load(Storage &storage)
{
    Number = storage["Modules"]["Behaviour"]["Number"].Default(2);
    Name = (string)storage["Modules"]["Behaviour"]["Names"][Number].Default("NAO");
}

void BehaviourSpell::Save(Storage &storage)
{
    storage["Modules"]["Behaviour"]["Number"] = Number;
    storage["Modules"]["Behaviour"]["Name"] = Name;
}