#pragma once

#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <functional>

class MirLib;

class MirLibFactory {
public:
    // 静态成员变量
    static std::atomic<bool> Loaded;
    static std::atomic<int> Progress;

    // 已预定义的资源库
    static std::unique_ptr<MirLib> ChrSel;
    static std::unique_ptr<MirLib> Prguse;
    static std::unique_ptr<MirLib> Prguse3;
    static std::unique_ptr<MirLib> BuffIcon;
    static std::unique_ptr<MirLib> Help;
    static std::unique_ptr<MirLib> MiniMap;
    static std::unique_ptr<MirLib> MagIcon;
    static std::unique_ptr<MirLib> MagIcon2;
    static std::unique_ptr<MirLib> Magic;
    static std::unique_ptr<MirLib> Magic2;
    static std::unique_ptr<MirLib> Magic3;
    static std::unique_ptr<MirLib> Effect;
    static std::unique_ptr<MirLib> MagicC;
    static std::unique_ptr<MirLib> GuildSkill;
    static std::unique_ptr<MirLib> Background;
    static std::unique_ptr<MirLib> Dragon;
    static std::unique_ptr<MirLib> Items;
    static std::unique_ptr<MirLib> StateItems;
    static std::unique_ptr<MirLib> FloorItems;
    static std::unique_ptr<MirLib> Deco;

    // 数组资源库
    static std::vector<std::unique_ptr<MirLib>> MapLibs;
    static std::vector<std::unique_ptr<MirLib>> WemadeMir2TilesExt;
    static std::vector<std::unique_ptr<MirLib>> Monsters;
    static std::vector<std::unique_ptr<MirLib>> Gates;
    static std::vector<std::unique_ptr<MirLib>> NPCs;
    static std::vector<std::unique_ptr<MirLib>> CArmours;
    static std::vector<std::unique_ptr<MirLib>> CHair;
    static std::vector<std::unique_ptr<MirLib>> CWeapons;
    static std::vector<std::unique_ptr<MirLib>> CHumEffect;
    static std::vector<std::unique_ptr<MirLib>> AArmours;
    static std::vector<std::unique_ptr<MirLib>> AHair;
    static std::vector<std::unique_ptr<MirLib>> AWeaponsL;
    static std::vector<std::unique_ptr<MirLib>> AWeaponsR;
    static std::vector<std::unique_ptr<MirLib>> AHumEffect;
    static std::vector<std::unique_ptr<MirLib>> ARArmours;
    static std::vector<std::unique_ptr<MirLib>> ARHair;
    static std::vector<std::unique_ptr<MirLib>> ARWeapons;
    static std::vector<std::unique_ptr<MirLib>> ARWeaponsS;
    static std::vector<std::unique_ptr<MirLib>> ARHumEffect;
    static std::vector<std::unique_ptr<MirLib>> Mounts;
    static std::vector<std::unique_ptr<MirLib>> Fishing;
    static std::vector<std::unique_ptr<MirLib>> Pets;
    static std::vector<std::unique_ptr<MirLib>> Transform;
    static std::vector<std::unique_ptr<MirLib>> TransformMounts;
    static std::vector<std::unique_ptr<MirLib>> TransformEffect;
    static std::vector<std::unique_ptr<MirLib>> TransformWeaponEffect;

    // 启动初始化
    static void StartToInit();

    // 获取地图图像
    static class MirImage* GetMirMapImage(int mapLibIdx, int imgIdx);

    // 绘图接口
    static void Draw(long targetSurface, MirLib* lib, int imgIdx, int x, int y);
    static void Draw(long targetSurface, MirLib* lib, int index, bool isBlend, int x, int y);
    static void Draw(long targetSurface, MirLib* lib, int imgIdx, const Rectangle& section, Point point, Color color, bool offSet);
    static void Draw(long targetSurface, MirLib* lib, int index, Point point, Color color, bool offSet);

private:
    static std::mutex mtx; // 锁用于线程安全

    // 私有方法
    static void LoadBaseLibraries();
    static void LoadGameLibraries();
    static void Run(); // 模拟 run 方法

    // 辅助函数
    static void TryDraw(long targetSurface, MirLib* lib, int imgIdx, ImageEffect effect, int x, int y,
                        const Rectangle* srcRect, bool isBlend, bool isOffset);
};
