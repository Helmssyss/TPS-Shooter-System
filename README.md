# TPS-Shooter-System
This (beginner) system is made with Unreal Engine 4.27

Developed with Blueprint and C++

Features:
- [x] Drop/Equip Weapons
- [x] Sprint/Walk
- [x] Switch weapons
- [x] Inventory
- [x] Reload system
- [x] Weapon(Rifle) Magazine Recharge
- [x] Fire Mode (Single-Auto)
- [x] Ragdoll Recover (without animation)
- [x] Head Rotation
- [x] Bullet Impact Particle
- [x] Bullet Decals


![Untitled](https://github.com/Helmssyss/TPS-Shooter-Game/assets/84701901/bf1e9fe9-be45-4130-b91a-6edc16a61206)

![Screenshot 2023-07-24 141359](https://github.com/Helmssyss/TPS-Shooter-System/assets/84701901/71b8f358-2a82-4a76-bff7-c9c3add86f24)

---

I did the reload event according to the following hierarchy. The hierarchy consists of two meshes and collisions. the appearance of the two meshes belongs to the weapon object. When the hand goes to the section where the magazine is, the main mesh is hidden, the fake mesh is revealed, and the magazine bone is hidden in the fake mesh. When a new magazine is added to the hand and going to the location of the weapon, this time the opposite happens.

![Screenshot 2023-07-04 183207](https://github.com/Helmssyss/TPS-Shooter-System/assets/84701901/986cc142-0a09-47c8-9d08-001b20293cfd)



https://github.com/Helmssyss/TPS-Shooter-System/assets/84701901/5b0e0d03-9876-49a8-9d1d-9a78d2d05004


[![Hits](https://hits.sh/github.com/Helmssyss/TPS-Shooter-Game.svg?label=views&color=007ec6)](https://hits.sh/github.com/Helmssyss/TPS-Shooter-Game/)
