#pragma once

namespace devilution {

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class SoundList : public std::enable_shared_from_this<Texture> {
public:
	static int None;
	static int Music;

	static int IntroMusic;
	static int SelectMusic;
	static int LoginEffect;

	static int ButtonA;
	static int ButtonB;
	static int ButtonC;
	static int Gold;
	static int EatDrug;
	static int ClickDrug;

	static int Teleport;
	static int LevelUp;

	static int ClickWeapon;
	static int ClickArmour;
	static int ClickRing;
	static int ClickBracelet;
	static int ClickNecklace;
	static int ClickHelmet;
	static int ClickBoots;
	static int ClickItem;

	// Movement
	static int WalkGroundL;
	static int WalkGroundR;
	static int RunGroundL;
	static int RunGroundR;
	static int WalkStoneL;
	static int WalkStoneR;
	static int RunStoneL;
	static int RunStoneR;
	static int WalkLawnL;
	static int WalkLawnR;
	static int RunLawnL;
	static int RunLawnR;
	static int WalkRoughL;
	static int WalkRoughR;
	static int RunRoughL;
	static int RunRoughR;
	static int WalkWoodL;
	static int WalkWoodR;
	static int RunWoodL;
	static int RunWoodR;
	static int WalkCaveL;
	static int WalkCaveR;
	static int RunCaveL;
	static int RunCaveR;
	static int WalkRoomL;
	static int WalkRoomR;
	static int RunRoomL;
	static int RunRoomR;
	static int WalkWaterL;
	static int WalkWaterR;
	static int RunWaterL;
	static int RunWaterR;

	// Weapon Swing
	static int SwingShort;
	static int SwingWood;
	static int SwingSword;
	static int SwingSword2;
	static int SwingAxe;
	static int SwingClub;
	static int SwingLong;
	static int SwingFist;

	// Struck
	static int StruckShort;
	static int StruckWooden;
	static int StruckSword;
	static int StruckSword2;
	static int StruckAxe;
	static int StruckClub;

	static int StruckBodySword;
	static int StruckBodyAxe;
	static int StruckBodyLongStick;
	static int StruckBodyFist;

	static int StruckArmourSword;
	static int StruckArmourAxe;
	static int StruckArmourLongStick;
	static int StruckArmourFist;

	static int MaleFlinch;
	static int FemaleFlinch;
	static int MaleDie;
	static int FemaleDie;

	static int Revive;
	static int ZombieRevive;

	// Mounts
	static int MountWalkL;
	static int MountWalkR;
	static int MountRun;
	static int TigerStruck1;
	static int TigerStruck2;
	static int TigerAttack1;
	static int TigerAttack2;
	static int TigerAttack3;

	static int FishingThrow;
	static int FishingPull;
	static int Fishing;

	static int WolfRide1;
	static int WolfRide2;
	static int WolfAttack1;
	static int WolfAttack2;
	static int WolfAttack3;
	static int WolfStruck1;
	static int WolfStruck2;
	static int TigerRide1;
	static int TigerRide2;

	static int PetPig;
	static int PetChick;
	static int PetKitty;
	static int PetSkeleton;
	static int PetPigman;
	static int PetWemade;
	static int PetBlackKitten;
	static int PetDragon;
	static int PetOlympic;
	static int PetFrog;
	static int PetMonkey;
	static int PetPickup;
};

int SoundList::None = 0;
int SoundList::Music = 0;

int SoundList::IntroMusic = 10146;
int SoundList::SelectMusic = 10147;
int SoundList::LoginEffect = 10100;

int SoundList::ButtonA = 10103;
int SoundList::ButtonB = 10104;
int SoundList::ButtonC = 10105;
int SoundList::Gold = 10106;
int SoundList::EatDrug = 10107;
int SoundList::ClickDrug = 10108;

int SoundList::Teleport = 10110;
int SoundList::LevelUp = 10156;

int SoundList::ClickWeapon = 10111;
int SoundList::ClickArmour = 10112;
int SoundList::ClickRing = 10113;
int SoundList::ClickBracelet = 10114;
int SoundList::ClickNecklace = 10115;
int SoundList::ClickHelmet = 10116;
int SoundList::ClickBoots = 10117;
int SoundList::ClickItem = 10118;

// Movement
int SoundList::WalkGroundL = 10001;
int SoundList::WalkGroundR = 10002;
int SoundList::RunGroundL = 10003;
int SoundList::RunGroundR = 10004;
int SoundList::WalkStoneL = 10005;
int SoundList::WalkStoneR = 10006;
int SoundList::RunStoneL = 10007;
int SoundList::RunStoneR = 10008;
int SoundList::WalkLawnL = 10009;
int SoundList::WalkLawnR = 10010;
int SoundList::RunLawnL = 10011;
int SoundList::RunLawnR = 10012;
int SoundList::WalkRoughL = 10013;
int SoundList::WalkRoughR = 10014;
int SoundList::RunRoughL = 10015;
int SoundList::RunRoughR = 10016;
int SoundList::WalkWoodL = 10017;
int SoundList::WalkWoodR = 10018;
int SoundList::RunWoodL = 10019;
int SoundList::RunWoodR = 10020;
int SoundList::WalkCaveL = 10021;
int SoundList::WalkCaveR = 10022;
int SoundList::RunCaveL = 10023;
int SoundList::RunCaveR = 10024;
int SoundList::WalkRoomL = 10025;
int SoundList::WalkRoomR = 10026;
int SoundList::RunRoomL = 10027;
int SoundList::RunRoomR = 10028;
int SoundList::WalkWaterL = 10029;
int SoundList::WalkWaterR = 10030;
int SoundList::RunWaterL = 10031;
int SoundList::RunWaterR = 10032;

// Weapon Swing
int SoundList::SwingShort = 10050;
int SoundList::SwingWood = 10051;
int SoundList::SwingSword = 10052;
int SoundList::SwingSword2 = 10053;
int SoundList::SwingAxe = 10054;
int SoundList::SwingClub = 10055;
int SoundList::SwingLong = 10056;
int SoundList::SwingFist = 10056;

// Struck
int SoundList::StruckShort = 10060;
int SoundList::StruckWooden = 10061;
int SoundList::StruckSword = 10062;
int SoundList::StruckSword2 = 10063;
int SoundList::StruckAxe = 10064;
int SoundList::StruckClub = 10065;

int SoundList::StruckBodySword = 10070;
int SoundList::StruckBodyAxe = 10071;
int SoundList::StruckBodyLongStick = 10072;
int SoundList::StruckBodyFist = 10073;

int SoundList::StruckArmourSword = 10080;
int SoundList::StruckArmourAxe = 10081;
int SoundList::StruckArmourLongStick = 10082;
int SoundList::StruckArmourFist = 10083;

int SoundList::MaleFlinch = 10138;
int SoundList::FemaleFlinch = 10139;
int SoundList::MaleDie = 10144;
int SoundList::FemaleDie = 10145;

int SoundList::Revive = 20791;
int SoundList::ZombieRevive = 0705;

// Mounts
int SoundList::MountWalkL = 10176;
int SoundList::MountWalkR = 10177;
int SoundList::MountRun = 10178;
int SoundList::TigerStruck1 = 10179;
int SoundList::TigerStruck2 = 10180;
int SoundList::TigerAttack1 = 10181;
int SoundList::TigerAttack2 = 10182;
int SoundList::TigerAttack3 = 10183;

int SoundList::FishingThrow = 10184;
int SoundList::FishingPull = 10185;
int SoundList::Fishing = 10186;

int SoundList::WolfRide1 = 10188;
int SoundList::WolfRide2 = 10189;
int SoundList::WolfAttack1 = 10190;
int SoundList::WolfAttack2 = 10191;
int SoundList::WolfAttack3 = 10192;
int SoundList::WolfStruck1 = 10193;
int SoundList::WolfStruck2 = 10194;
int SoundList::TigerRide1 = 10218;
int SoundList::TigerRide2 = 10219;

int SoundList::PetPig = 10500;
int SoundList::PetChick = 10501;
int SoundList::PetKitty = 10502;
int SoundList::PetSkeleton = 10503;
int SoundList::PetPigman = 10504;
int SoundList::PetWemade = 10505;
int SoundList::PetBlackKitten = 10506;
int SoundList::PetDragon = 10507;
int SoundList::PetOlympic = 10508;
int SoundList::PetFrog = 10510;
int SoundList::PetMonkey = 10511;
int SoundList::PetPickup = 10520;

} // namespace devilution
