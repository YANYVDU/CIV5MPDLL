-- ******************** UNIQUE CITYSTATES：DiplomaticPrestige******************** --
alter table Buildings add DiplomaticPrestige int default 0;
alter table Policies  add DiplomaticPrestige int default 0;
alter table Traits    add DiplomaticPrestige int default 0;

-- Diplomatic Overextension Penalty Ratios (GlobalDefines, Rule 20)
INSERT INTO Defines (Name, Value) VALUES ('DIPLOMATIC_OVEREXTENSION_DECAY_MODIFIER', 10);
INSERT INTO Defines (Name, Value) VALUES ('DIPLOMATIC_OVEREXTENSION_RISE_MODIFIER', -10);
INSERT INTO Defines (Name, Value) VALUES ('DIPLOMATIC_OVEREXTENSION_UNHAPPINESS_MODIFIER', 3);

-- City-State UA Basic Effects (GlobalDefines, per-ally modifier values)
-- Cultured
INSERT INTO Defines (Name, Value) VALUES ('CS_CULTURED_POLICY_COST_MODIFIER', -2);
INSERT INTO Defines (Name, Value) VALUES ('CS_CULTURED_IMMIGRATION_REGRESSAND_MODIFIER', -2);
-- Militaristic
INSERT INTO Defines (Name, Value) VALUES ('CS_MILITARISTIC_LAND_XP_PER_TURN', 1);
-- Maritime
INSERT INTO Defines (Name, Value) VALUES ('CS_MARITIME_SEA_TRADE_GOLD_PER_ERA', 1);
-- Religious
INSERT INTO Defines (Name, Value) VALUES ('CS_RELIGIOUS_FAITH_COST_MODIFIER', -5);
INSERT INTO Defines (Name, Value) VALUES ('CS_RELIGIOUS_PRESSURE_MODIFIER', 10);
-- Mercantile
INSERT INTO Defines (Name, Value) VALUES ('CS_MERCANTILE_LUXURY_HAPPINESS_MODIFIER', 5);
INSERT INTO Defines (Name, Value) VALUES ('CS_MERCANTILE_TREASURY_INTEREST_RATE', 1);
INSERT INTO Defines (Name, Value) VALUES ('CS_TREASURY_INTEREST_CAP_MULTIPLIER', 100);


-- MinorCivAlliesThresholdExtra: per-era ally threshold increase (Rule 8)
alter table Eras add column MinorCivAlliesThresholdExtra int default 0;

-- MinorCivAlliesThresholdModifier: per-building/policy/trait threshold modifier
alter table Buildings add column MinorCivAlliesThresholdModifier int default 0;
alter table Policies  add column MinorCivAlliesThresholdModifier int default 0;
alter table Traits    add column MinorCivAlliesThresholdModifier int default 0;

-- ==================== Unique CityState UA System (Rule 1-18) ====================

-- 效果定义表（内部数据，不展示给玩家）：每个盟友效果或朋友效果一行
-- 列名按来源城邦分组注释，0 表示该效果不适用
CREATE TABLE CityStateUAEffects (
    ID                                              INTEGER PRIMARY KEY AUTOINCREMENT,
    Type                                            TEXT NOT NULL UNIQUE,
    -- 佛罗伦萨：信仰购买伟人成本「上涨幅度（增量）」降速折扣（仅作用于增量，非整体成本）
    FaithPurchaseGreatPeopleCostRiseModifier       integer DEFAULT 0,
    FaithPurchaseGreatPeopleCostRiseModifierPerGW  integer DEFAULT 0,
    FaithPurchaseAllGreatPeople                     boolean DEFAULT 0,
    -- 布宜诺斯艾利斯：大音乐家不死+保留演唱会魅力
    GPNoDeathAfterGreatWork                         boolean DEFAULT 0,
    GPConcertTourismRetentionPercent                integer DEFAULT 0,
    -- 布鲁塞尔：大音乐家演唱会特殊修正
    GreatMusicianConcertTourismModifier             integer DEFAULT 0,
    GreatMusicianConcertGoldPercent                 integer DEFAULT 0,
    -- 布拉迪斯拉发：首都及第二首都
    CapitalAndSecondCapitalCultureModifier          integer DEFAULT 0,
    -- 基辅：首都每回合累积
    CapitalCultureModifierPerTurn                   integer DEFAULT 0,
    CapitalFaithModifierPerTurn                     integer DEFAULT 0,
    CapitalPerTurnYieldModifierMax                  integer DEFAULT 0,
    -- 布加勒斯特：国际移民
    ImmigrationRatePerImmigrant                     integer DEFAULT 0,
    ImmigrationRateMax                              integer DEFAULT 0,
    EmigrationRatePerImmigrant                      integer DEFAULT 0,
    EmigrationRateMax                               integer DEFAULT 0,
    -- 吉隆坡：傀儡城市科研阈值
    PuppetNoTechCostPenalty                         boolean DEFAULT 0,
    PuppetTechCostPartial                           integer DEFAULT 0,
    -- 阿拉木图：掠夺商路
    CanPillageNeutralTradeRoute                     boolean DEFAULT 0,
    -- 贝尔格莱德：驻守城防
    GarrisonCityDefenseModifier                     integer DEFAULT 0,
    -- 布达佩斯：渡河免疫
    LandUnitsImmuneRiverCrossing                    boolean DEFAULT 0,
    -- 河内：固定伤害+和谈+被宣
    EnemyFixedDamageModifierInBorders               integer DEFAULT 0,
    CulturePerWarPeace                              integer DEFAULT 0,
    EnemyCombatModifierInBordersPerBeenDoW          integer DEFAULT 0,
    -- 姆班扎刚果：城市数量相关
    UnitProductionModifierPerCity                   integer DEFAULT 0,
    ManpowerPerCity                                 integer DEFAULT 0,
    CombatBonusPerTechDifference                    integer DEFAULT 0,
    -- 西顿：海军攻城+境外回血
    NavalAttackIgnoreBuildingDefense                integer DEFAULT 0,
    ForeignRegenPercent                             integer DEFAULT 0,
    -- 索非亚：丘陵城市
    HillsCityDamageReduction                        integer DEFAULT 0,
    HillsMovementModifier                           integer DEFAULT 0,
    HillsCityRangeBonus                             integer DEFAULT 0,
    -- 梵蒂冈：宗教传播速度
    ReligionSpreadSpeedModifier                     integer DEFAULT 0,
    -- 克孜勒：商路上限→商路距离
    LandTradeRouteDistancePerTradeSlot              integer DEFAULT 0,
    -- 迪拜：捐赠计数
    HappinessPerGoldDonated                         integer DEFAULT 0,
    GoldDonationInterval                            integer DEFAULT 0,
    WonderProductionPerDonationHappiness            integer DEFAULT 0,
    -- 马六甲：奢侈品快乐
    LuxuryHappinessModifier                         integer DEFAULT 0
);

-- UA类型表（展示给玩家）：将一个城邦的盟友效果和朋友效果配对
-- Description = 盟友效果文本  Help = 朋友效果文本，UI层拼合展示
CREATE TABLE CityStateUAs (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Type TEXT NOT NULL UNIQUE,
    Description TEXT DEFAULT NULL REFERENCES Language_en_US(Tag),
    Help TEXT DEFAULT NULL REFERENCES Language_en_US(Tag),
    AllyEffectType TEXT NOT NULL REFERENCES CityStateUAEffects(Type),
    FriendEffectType TEXT NOT NULL REFERENCES CityStateUAEffects(Type)
);

-- MinorCivilizations 直接挂 UAType（无需映射表）
alter table MinorCivilizations add column UAType text default null references CityStateUAs(Type);
-- CityState UA: adds per-turn Great Person Points (per SpecialistType) to all cities
create table CityStateUAEffect_GreatPersonPoints (
    EffectType text references CityStateUAEffects(Type),
    SpecialistType text references Specialists(Type),
    Points integer default 0
);

