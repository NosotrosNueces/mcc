/* Blocks */

typedef enum {
    RC_BLOCK_AIR,
    MC_BLOCK_STONE,
    MC_BLOCK_GRASS,
    MC_BLOCK_DIRT,
    MC_BLOCK_COBBLESTONE,
    MC_BLOCK_PLANKS,
    MC_BLOCK_SAPLING,
    MC_BLOCK_BEDROCK,
    MC_BLOCK_FLOWING_WATER,
    MC_BLOCK_WATER,
    MC_BLOCK_FLOWING_LAVA,
    MC_BLOCK_LAVA,
    MC_BLOCK_SAND,
    MC_BLOCK_GRAVEL,
    MC_BLOCK_GOLD_ORE,
    MC_BLOCK_IRON_ORE,
    MC_BLOCK_COAL_ORE,
    MC_BLOCK_LOG,
    MC_BLOCK_LEAVES,
    MC_BLOCK_SPONGE,
    MC_BLOCK_GLASS,
    MC_BLOCK_LAPIS_ORE,
    MC_BLOCK_LAPIS_BLOCK,
    MC_BLOCK_DISPENSER,
    MC_BLOCK_SANDSTONE,
    MC_BLOCK_NOTEBLOCK,
    MC_BLOCK_BED,
    MC_BLOCK_GOLDEN_RAIL,
    MC_BLOCK_DETECTOR_RAIL,
    MC_BLOCK_STICKY_PISTON,
    MC_BLOCK_WEB,
    MC_BLOCK_TALLGRASS,
    MC_BLOCK_DEADBUSH,
    MC_BLOCK_PISTON,
    MC_BLOCK_PISTON_HEAD,
    MC_BLOCK_WOOL,
    MC_BLOCK_PISTON_EXTENSION,
    MC_BLOCK_YELLOW_FLOWER,
    MC_BLOCK_RED_FLOWER,
    MC_BLOCK_BROWN_MUSHROOM,
    MC_BLOCK_RED_MUSHROOM,
    MC_BLOCK_GOLD_BLOCK,
    MC_BLOCK_IRON_BLOCK,
    MC_BLOCK_DOUBLE_STONE_SLAB,
    MC_BLOCK_STONE_SLAB,
    MC_BLOCK_BRICK_BLOCK,
    MC_BLOCK_TNT,
    MC_BLOCK_BOOKSHELF,
    MC_BLOCK_MOSSY_COBBLESTONE,
    MC_BLOCK_OBSIDIAN,
    MC_BLOCK_TORCH,
    MC_BLOCK_FIRE,
    MC_BLOCK_MOB_SPAWNER,
    MC_BLOCK_OAK_STAIRS,
    MC_BLOCK_CHEST,
    MC_BLOCK_REDSTONE_WIRE,
    MC_BLOCK_DIAMOND_ORE,
    MC_BLOCK_DIAMOND_BLOCK,
    MC_BLOCK_CRAFTING_TABLE,
    MC_BLOCK_WHEAT,
    MC_BLOCK_FARMLAND,
    MC_BLOCK_FURNACE,
    MC_BLOCK_LIT_FURNACE,
    MC_BLOCK_STANDING_SIGN,
    MC_BLOCK_WOODEN_DOOR,
    MC_BLOCK_LADDER,
    MC_BLOCK_RAIL,
    MC_BLOCK_STONE_STAIRS,
    MC_BLOCK_WALL_SIGN,
    MC_BLOCK_LEVER,
    MC_BLOCK_STONE_PRESSURE_PLATE,
    MC_BLOCK_IRON_DOOR,
    MC_BLOCK_WOODEN_PRESSURE_PLATE,
    MC_BLOCK_REDSTONE_ORE,
    MC_BLOCK_LIT_REDSTONE_ORE,
    MC_BLOCK_UNLIT_REDSTONE_TORCH,
    MC_BLOCK_REDSTONE_TORCH,
    MC_BLOCK_STONE_BUTTON,
    MC_BLOCK_SNOW_LAYER,
    MC_BLOCK_ICE,
    MC_BLOCK_SNOW,
    MC_BLOCK_CACTUS,
    MC_BLOCK_CLAY,
    MC_BLOCK_REEDS,
    MC_BLOCK_JUKEBOX,
    MC_BLOCK_FENCE,
    MC_BLOCK_PUMPKIN,
    MC_BLOCK_NETHERRACK,
    MC_BLOCK_SOUL_SAND,
    MC_BLOCK_GLOWSTONE,
    MC_BLOCK_PORTAL,
    MC_BLOCK_LIT_PUMPKIN,
    MC_BLOCK_CAKE,
    MC_BLOCK_UNPOWERED_REPEATER,
    MC_BLOCK_POWERED_REPEATER,
    MC_BLOCK_STAINED_GLASS,
    MC_BLOCK_TRAPDOOR,
    MC_BLOCK_MONSTER_EGG,
    MC_BLOCK_STONEBRICK,
    MC_BLOCK_BROWN_MUSHROOM_BLOCK,
    MC_BLOCK_RED_MUSHROOM_BLOCK,
    MC_BLOCK_IRON_BARS,
    MC_BLOCK_GLASS_PANE,
    MC_BLOCK_MELON_BLOCK,
    MC_BLOCK_PUMPKIN_STEM,
    MC_BLOCK_MELON_STEM,
    MC_BLOCK_VINE,
    MC_BLOCK_FENCE_GATE,
    MC_BLOCK_BRICK_STAIRS,
    MC_BLOCK_STONE_BRICK_STAIRS,
    MC_BLOCK_MYCELIUM,
    MC_BLOCK_WATERLILY,
    MC_BLOCK_NETHER_BRICK,
    MC_BLOCK_NETHER_BRICK_FENCE,
    MC_BLOCK_NETHER_BRICK_STAIRS,
    MC_BLOCK_NETHER_WART,
    MC_BLOCK_ENCHANTING_TABLE,
    MC_BLOCK_BREWING_STAND,
    MC_BLOCK_CAULDRON,
    MC_BLOCK_END_PORTAL,
    MC_BLOCK_END_PORTAL_FRAME,
    MC_BLOCK_END_STONE,
    MC_BLOCK_DRAGON_EGG,
    MC_BLOCK_REDSTONE_LAMP,
    MC_BLOCK_LIT_REDSTONE_LAMP,
    MC_BLOCK_DOUBLE_WOODEN_SLAB,
    MC_BLOCK_WOODEN_SLAB,
    MC_BLOCK_COCOA,
    MC_BLOCK_SANDSTONE_STAIRS,
    MC_BLOCK_EMERALD_ORE,
    MC_BLOCK_ENDER_CHEST,
    MC_BLOCK_TRIPWIRE_HOOK,
    MC_BLOCK_TRIPWIRE,
    MC_BLOCK_EMERALD_BLOCK,
    MC_BLOCK_SPRUCE_STAIRS,
    MC_BLOCK_BIRCH_STAIRS,
    MC_BLOCK_JUNGLE_STAIRS,
    MC_BLOCK_COMMAND_BLOCK,
    MC_BLOCK_BEACON,
    MC_BLOCK_COBBLESTONE_WALL,
    MC_BLOCK_FLOWER_POT,
    MC_BLOCK_CARROTS,
    MC_BLOCK_POTATOES,
    MC_BLOCK_WOODEN_BUTTON,
    MC_BLOCK_SKULL,
    MC_BLOCK_ANVIL,
    MC_BLOCK_TRAPPED_CHEST,
    MC_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE,
    MC_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE,
    MC_BLOCK_UNPOWERED_COMPARATOR,
    MC_BLOCK_POWERED_COMPARATOR,
    MC_BLOCK_DAYLIGHT_DETECTOR,
    MC_BLOCK_REDSTONE_BLOCK,
    MC_BLOCK_QUARTZ_ORE,
    MC_BLOCK_HOPPER,
    MC_BLOCK_QUARTZ_BLOCK,
    MC_BLOCK_QUARTZ_STAIRS,
    MC_BLOCK_ACTIVATOR_RAIL,
    MC_BLOCK_DROPPER,
    MC_BLOCK_STAINED_HARDENED_CLAY,
    MC_BLOCK_STAINED_GLASS_PANE,
    MC_BLOCK_LEAVES2,
    MC_BLOCK_LOG2,
    MC_BLOCK_ACACIA_STAIRS,
    MC_BLOCK_DARK_OAK_STAIRS,
    MC_BLOCK_SLIME,
    MC_BLOCK_BARRIER,
    MC_BLOCK_IRON_TRAPDOOR,
    MC_BLOCK_PRISMARINE,
    MC_BLOCK_SEA_LANTERN,
    MC_BLOCK_HAY_BLOCK,
    MC_BLOCK_CARPET,
    MC_BLOCK_HARDENED_CLAY,
    MC_BLOCK_COAL_BLOCK,
    MC_BLOCK_PACKED_ICE,
    MC_BLOCK_DOUBLE_PLANT,
    MC_BLOCK_STANDING_BANNER,
    MC_BLOCK_WALL_BANNER,
    MC_BLOCK_DAYLIGHT_DETECTOR_INVERTED,
    MC_BLOCK_RED_SANDSTONE,
    MC_BLOCK_RED_SANDSTONE_STAIRS,
    MC_BLOCK_DOUBLE_STONE_SLAB2,
    MC_BLOCK_STONE_SLAB2,
    MC_BLOCK_SPRUCE_FENCE_GATE,
    MC_BLOCK_BIRCH_FENCE_GATE,
    MC_BLOCK_JUNGLE_FENCE_GATE,
    MC_BLOCK_DARK_OAK_FENCE_GATE,
    MC_BLOCK_ACACIA_FENCE_GATE,
    MC_BLOCK_SPRUCE_FENCE,
    MC_BLOCK_BIRCH_FENCE,
    MC_BLOCK_JUNGLE_FENCE,
    MC_BLOCK_DARK_OAK_FENCE,
    MC_BLOCK_ACACIA_FENCE,
    MC_BLOCK_SPRUCE_DOOR,
    MC_BLOCK_BIRCH_DOOR,
    MC_BLOCK_JUNGLE_DOOR,
    MC_BLOCK_ACACIA_DOOR,
    MC_BLOCK_DARK_OAK_DOOR
} block;

/* Items */

typedef enum {
    MC_ITEM_IRON_SHOVEL = 0x100,
    MC_ITEM_IRON_PICKAXE,
    MC_ITEM_IRON_AXE,
    MC_ITEM_FLINT_AND_STEEL,
    MC_ITEM_APPLE,
    MC_ITEM_BOW,
    MC_ITEM_ARROW,
    MC_ITEM_COAL,
    MC_ITEM_DIAMOND,
    MC_ITEM_IRON_INGOT,
    MC_ITEM_GOLD_INGOT,
    MC_ITEM_IRON_SWORD,
    MC_ITEM_WOODEN_SWORD,
    MC_ITEM_WOODEN_SHOVEL,
    MC_ITEM_WOODEN_PICKAXE,
    MC_ITEM_WOODEN_AXE,
    MC_ITEM_STONE_SWORD,
    MC_ITEM_STONE_SHOVEL,
    MC_ITEM_STONE_PICKAXE,
    MC_ITEM_STONE_AXE,
    MC_ITEM_DIAMOND_SWORD,
    MC_ITEM_DIAMOND_SHOVEL,
    MC_ITEM_DIAMOND_PICKAXE,
    MC_ITEM_DIAMOND_AXE,
    MC_ITEM_STICK,
    MC_ITEM_BOWL,
    MC_ITEM_MUSHROOM_STEW,
    MC_ITEM_GOLDEN_SWORD,
    MC_ITEM_GOLDEN_SHOVEL,
    MC_ITEM_GOLDEN_PICKAXE,
    MC_ITEM_GOLDEN_AXE,
    MC_ITEM_STRING,
    MC_ITEM_FEATHER,
    MC_ITEM_GUNPOWDER,
    MC_ITEM_WOODEN_HOE,
    MC_ITEM_STONE_HOE,
    MC_ITEM_IRON_HOE,
    MC_ITEM_DIAMOND_HOE,
    MC_ITEM_GOLDEN_HOE,
    MC_ITEM_WHEAT_SEEDS,
    MC_ITEM_WHEAT,
    MC_ITEM_BREAD,
    MC_ITEM_LEATHER_HELMET,
    MC_ITEM_LEATHER_CHESTPLATE,
    MC_ITEM_LEATHER_LEGGINGS,
    MC_ITEM_LEATHER_BOOTS,
    MC_ITEM_CHAINMAIL_HELMET,
    MC_ITEM_CHAINMAIL_CHESTPLATE,
    MC_ITEM_CHAINMAIL_LEGGINGS,
    MC_ITEM_CHAINMAIL_BOOTS,
    MC_ITEM_IRON_HELMET,
    MC_ITEM_IRON_CHESTPLATE,
    MC_ITEM_IRON_LEGGINGS,
    MC_ITEM_IRON_BOOTS,
    MC_ITEM_DIAMOND_HELMET,
    MC_ITEM_DIAMOND_CHESTPLATE,
    MC_ITEM_DIAMOND_LEGGINGS,
    MC_ITEM_DIAMOND_BOOTS,
    MC_ITEM_GOLDEN_HELMET,
    MC_ITEM_GOLDEN_CHESTPLATE,
    MC_ITEM_GOLDEN_LEGGINGS,
    MC_ITEM_GOLDEN_BOOTS,
    MC_ITEM_FLINT,
    MC_ITEM_PORKCHOP,
    MC_ITEM_COOKED_PORKCHOP,
    MC_ITEM_PAINTING,
    MC_ITEM_GOLDEN_APPLE,
    MC_ITEM_SIGN,
    MC_ITEM_WOODEN_DOOR,
    MC_ITEM_BUCKET,
    MC_ITEM_WATER_BUCKET,
    MC_ITEM_LAVA_BUCKET,
    MC_ITEM_MINECART,
    MC_ITEM_SADDLE,
    MC_ITEM_IRON_DOOR,
    MC_ITEM_REDSTONE,
    MC_ITEM_SNOWBALL,
    MC_ITEM_BOAT,
    MC_ITEM_LEATHER,
    MC_ITEM_MILK_BUCKET,
    MC_ITEM_BRICK,
    MC_ITEM_CLAY_BALL,
    MC_ITEM_REEDS,
    MC_ITEM_PAPER,
    MC_ITEM_BOOK,
    MC_ITEM_SLIME_BALL,
    MC_ITEM_CHEST_MINECART,
    MC_ITEM_FURNACE_MINECART,
    MC_ITEM_EGG,
    MC_ITEM_COMPASS,
    MC_ITEM_FISHING_ROD,
    MC_ITEM_CLOCK,
    MC_ITEM_GLOWSTONE_DUST,
    MC_ITEM_FISH,
    MC_ITEM_COOKED_FISH,
    MC_ITEM_DYE,
    MC_ITEM_BONE,
    MC_ITEM_SUGAR,
    MC_ITEM_CAKE,
    MC_ITEM_BED,
    MC_ITEM_REPEATER,
    MC_ITEM_COOKIE,
    MC_ITEM_FILLED_MAP,
    MC_ITEM_SHEARS,
    MC_ITEM_MELON,
    MC_ITEM_PUMPKIN_SEEDS,
    MC_ITEM_MELON_SEEDS,
    MC_ITEM_BEEF,
    MC_ITEM_COOKED_BEEF,
    MC_ITEM_CHICKEN,
    MC_ITEM_COOKED_CHICKEN,
    MC_ITEM_ROTTEN_FLESH,
    MC_ITEM_ENDER_PEARL,
    MC_ITEM_BLAZE_ROD,
    MC_ITEM_GHAST_TEAR,
    MC_ITEM_GOLD_NUGGET,
    MC_ITEM_NETHER_WART,
    MC_ITEM_POTION,
    MC_ITEM_GLASS_BOTTLE,
    MC_ITEM_SPIDER_EYE,
    MC_ITEM_FERMENTED_SPIDER_EYE,
    MC_ITEM_BLAZE_POWDER,
    MC_ITEM_MAGMA_CREAM,
    MC_ITEM_BREWING_STAND,
    MC_ITEM_CAULDRON,
    MC_ITEM_ENDER_EYE,
    MC_ITEM_SPECKLED_MELON,
    MC_ITEM_SPAWN_EGG,
    MC_ITEM_EXPERIENCE_BOTTLE,
    MC_ITEM_FIRE_CHARGE,
    MC_ITEM_WRITABLE_BOOK,
    MC_ITEM_WRITTEN_BOOK,
    MC_ITEM_EMERALD,
    MC_ITEM_ITEM_FRAME,
    MC_ITEM_FLOWER_POT,
    MC_ITEM_CARROT,
    MC_ITEM_POTATO,
    MC_ITEM_BAKED_POTATO,
    MC_ITEM_POISONOUS_POTATO,
    MC_ITEM_MAP,
    MC_ITEM_GOLDEN_CARROT,
    MC_ITEM_SKULL,
    MC_ITEM_CARROT_ON_A_STICK,
    MC_ITEM_NETHER_STAR,
    MC_ITEM_PUMPKIN_PIE,
    MC_ITEM_FIREWORKS,
    MC_ITEM_FIREWORK_CHARGE,
    MC_ITEM_ENCHANTED_BOOK,
    MC_ITEM_COMPARATOR,
    MC_ITEM_NETHERBRICK,
    MC_ITEM_QUARTZ,
    MC_ITEM_TNT_MINECART,
    MC_ITEM_HOPPER_MINECART,
    MC_ITEM_PRISMARINE_SHARD,
    MC_ITEM_PRISMARINE_CRYSTALS,
    MC_ITEM_RABBIT,
    MC_ITEM_COOKED_RABBIT,
    MC_ITEM_RABBIT_STEW,
    MC_ITEM_RABBIT_FOOT,
    MC_ITEM_RABBIT_HIDE,
    MC_ITEM_ARMOR_STAND,
    MC_ITEM_IRON_HORSE_ARMOR,
    MC_ITEM_GOLDEN_HORSE_ARMOR,
    MC_ITEM_DIAMOND_HORSE_ARMOR,
    MC_ITEM_LEAD,
    MC_ITEM_NAME_TAG,
    MC_ITEM_COMMAND_BLOCK_MINECART,
    MC_ITEM_MUTTON,
    MC_ITEM_COOKED_MUTTON,
    MC_ITEM_BANNER,
    MC_ITEM_SPRUCE_DOOR,
    MC_ITEM_BIRCH_DOOR,
    MC_ITEM_JUNGLE_DOOR,
    MC_ITEM_ACACIA_DOOR,
    MC_ITEM_DARK_OAK_DOOR
} item;

/* Music Records */

#define MC_RECORD_13         0x8D0
#define MC_RECORD_CAT        0x8D1
#define MC_RECORD_BLOCKS     0x8D2
#define MC_RECORD_CHIRP      0x8D3
#define MC_RECORD_FAR        0x8D4
#define MC_RECORD_MALL       0x8D5
#define MC_RECORD_MELLOHI    0x8D6
#define MC_RECORD_STAL       0x8D7
#define MC_RECORD_STRAD      0x8D8
#define MC_RECORD_WARD       0x8D9
#define MC_RECORD_11         0x8DA
#define MC_RECORD_WAIT       0x8DB

/* Entities */

#define ITEM                        0x1
#define XPORB                       0x2
#define LEASHKNOT                   0x8
#define PAINTING                    0x9
#define ARROW                       0xA
#define SNOWBALL                    0xB
#define FIREBALL                    0xC
#define SMALLFIREBALL               0xD
#define THROWNENDERPEARL            0xE
#define EYEOFENDERSIGNAL            0xF
#define THROWNPOTION                0x10
#define THROWNEXPBOTTLE             0x11
#define ITEMFRAME                   0x12
#define WITHERSKULL                 0x13
#define PRIMEDTNT                   0x14
#define FALLINGSAND                 0x15
#define FIREWORKSROCKETENTITY       0x16
#define ARMORSTAND                  0x1E
#define MINECARTCOMMANDBLOCK        0x28
#define BOAT                        0x29
#define MINECARTRIDEABLE            0x2A
#define MINECARTCHEST               0x2B
#define MINECARTFURNACE             0x2C
#define MINECARTTNT                 0x2D
#define MINECARTHOPPER              0x2E
#define MINECARTSPAWNER             0x2F
#define MOB                         0x30
#define MONSTER                     0x31
#define CREEPER                     0x32
#define SKELETON                    0x33
#define SPIDER                      0x34
#define GIANT                       0x35
#define ZOMBIE                      0x36
#define SLIME                       0x37
#define GHAST                       0x38
#define PIGZOMBIE                   0x39
#define ENDERMAN                    0x3A
#define CAVESPIDER                  0x3B
#define SILVERFISH                  0x3C
#define BLAZE                       0x3D
#define LAVASLIME                   0x3E
#define ENDERDRAGON                 0x3F
#define WITHERBOSS                  0x40
#define BAT                         0x41
#define WITCH                       0x42
#define ENDERMITE                   0x43
#define GUARDIAN                    0x44
#define PIG                         0x5A
#define SHEEP                       0x5B
#define COW                         0x5C
#define CHICKEN                     0x5D
#define SQUID                       0x5E
#define WOLF                        0x5F
#define MUSHROOMCOW                 0x60
#define SNOWMAN                     0x61
#define OZELOT                      0x62
#define VILLAGERGOLEM               0x63
#define ENTITYHORSE                 0x64
#define RABBIT                      0x65
#define RABBIT                      0x65
#define VILLAGER                    0x78
#define ENDERCRYSTAL                0xC8

/* Biomes */

#define OCEAN                   0x0
#define PLAINS                  0x1
#define DESERT                  0x2
#define EXTREME_HILLS           0x3
#define FOREST                  0x4
#define TAIGA                   0x5
#define SWAMPLAND               0x6
#define RIVER                   0x7
#define NETHER                  0x8
#define END                     0x9
#define FROZEN_OCEAN            0x10
#define FROZEN_RIVER            0x11
#define ICE_PLAINS              0x12
#define ICE_MOUNTAINS           0x13
#define MUSHROOM_ISLAND         0x14
#define MUSHROOM_ISLAND_SHORE   0x15
#define BEACH                   0x16
#define DESERT_HILLS            0x17
#define FOREST_HILLS            0x18
#define TAIGA_HILLS             0x19
#define EXTREME_HILLS_EDGE      0x20
#define JUNGLE                  0x21
#define JUNGLE_HILLS            0x22
#define JUNGLE_EDGE             0x23
#define DEEP_OCEAN              0x24
#define STONE_BEACH             0x25
#define COLD_BEACH              0x26
#define BIRCH_FOREST            0x27
#define BIRCH_FOREST_HILLS      0x28
#define ROOFED_FOREST           0x29
#define COLD_TAIGA              0x30
#define COLD_TAIGA_HILLS        0x31
#define MEGA_TAIGA              0x32
#define MEGA_TAIGA_HILLS        0x33
#define EXTREME_HILLS+          0x34
#define SAVANNA                 0x35
#define SAVANNA_PLATEAU         0x36
#define MESA                    0x37
#define MESA_PLATEAU_F          0x38
#define MESA_PLATEAU            0x39
#define SUNFLOWER_PLAINS        0x129
#define DESERT_M                0x130
#define EXTREME_HILLS_M         0x131
#define FLOWER_FOREST           0x132
#define TAIGA_M                 0x133
#define SWAMPLAND_M             0x134
#define ICE_PLAINS_SPIKES       0x140
#define JUNGLE_M                0x149
#define JUNGLEEDGE_M            0x151
#define BIRCH_FOREST_M          0x155
#define BIRCH_FOREST_HILLS_M    0x156
#define ROOFED_FOREST_M         0x157
#define COLD_TAIGA_M            0x158
#define MEGA_SPRUCE_TAIGA       0x160
#define MEGA_SPRUCE_TAIGA_HILLS 0x161
#define EXTREME_HILLS+ M        0x162
#define SAVANNA_M               0x163
#define SAVANNA_PLATEAU_M       0x164
#define MESA (BRYCE)            0x165
#define MESA_PLATEAU_F M        0x166
#define MESA_PLATEAU_M          0x167

/* Enchantments */

#define MINECRAFT_PROTECTION                0x0
#define MINECRAFT_FIRE_PROTECTION           0x1
#define MINECRAFT_FEATHER_FALLING           0x2
#define MINECRAFT_BLAST_PROTECTION          0x3
#define MINECRAFT_PROJECTILE_PROTECTION     0x4
#define MINECRAFT_RESPIRATION               0x5
#define MINECRAFT_AQUA_AFFINITY             0x6
#define MINECRAFT_THORNS                    0x7
#define MINECRAFT_DEPTH_STRIDER             0x8
#define MINECRAFT_SHARPNESS                 0x16
#define MINECRAFT_SMITE SMITE               0x17
#define MINECRAFT_BANE_OF_ARTHROPODS        0x18
#define MINECRAFT_KNOCKBACK                 0x19
#define MINECRAFT_FIRE_ASPECT               0x20
#define MINECRAFT_LOOTING                   0x21
#define MINECRAFT_EFFICIENCY                0x32
#define MINECRAFT_SILK_TOUCH                0x33
#define MINECRAFT_UNBREAKING                0x34
#define MINECRAFT_FORTUNE                   0x35
#define MINECRAFT_POWER POWER               0x48
#define MINECRAFT_PUNCH PUNCH               0x49
#define MINECRAFT_FLAME FLAME               0x50
#define MINECRAFT_INFINITY                  0x51
#define MINECRAFT_LUCK_OF_THE_SEA           0x61
#define MINECRAFT_LURE                      0x62
#define MINECRAFT_PROTECTION_MAX                4
#define MINECRAFT_FIRE_PROTECTION_MAX           4
#define MINECRAFT_FEATHER_FALLING_MAX           4
#define MINECRAFT_BLAST_PROTECTION_MAX          4
#define MINECRAFT_PROJECTILE_PROTECTION_MAX     4
#define MINECRAFT_RESPIRATION_MAX               3
#define MINECRAFT_AQUA_AFFINITY_MAX             1
#define MINECRAFT_THORNS_MAX                    3
#define MINECRAFT_DEPTH_STRIDER_MAX             3
#define MINECRAFT_SHARPNESS_MAX                 5
#define MINECRAFT_SMITE_MAX                     5
#define MINECRAFT_BANE_OF_ARTHROPODS_MAX        5
#define MINECRAFT_KNOCKBACK_MAX                 2
#define MINECRAFT_FIRE_ASPECT_MAX               2
#define MINECRAFT_LOOTING_MAX                   3
#define MINECRAFT_EFFICIENCY_MAX                5
#define MINECRAFT_SILK_TOUCH_MAX                1
#define MINECRAFT_UNBREAKING_MAX                3
#define MINECRAFT_FORTUNE_MAX                   3
#define MINECRAFT_POWER_MAX                     5
#define MINECRAFT_PUNCH_MAX                     2
#define MINECRAFT_FLAME_MAX                     1
#define MINECRAFT_INFINITY_MAX                  1
#define MINECRAFT_LUCK_OF_THE_SEA_MAX           3
#define MINECRAFT_LURE_MAX                      3

/* Effects */

#define minecraft:speed                 1
#define minecraft:slowness              2
#define minecraft:haste                 3
#define minecraft:mining_fatigue        4
#define minecraft:strength              5
#define minecraft:instant_health        6
#define minecraft:instant_damage        7
#define minecraft:jump_boost            8
#define minecraft:nausea                9
#define minecraft:regeneration          10
#define minecraft:resistance            11
#define minecraft:fire_resistance       12
#define minecraft:water_breathing       13
#define minecraft:invisibility          14
#define minecraft:blindness             15
#define minecraft:night_vision          16
#define minecraft:hunger                17
#define minecraft:weakness              18
#define minecraft:poison                19
#define minecraft:wither                20
#define minecraft:health_boost          21
#define minecraft:absorption            22
#define minecraft:saturation            23

/* more to come */
