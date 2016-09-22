#pragma once

/* Minecraft Data Values: http://minecraft.gamepedia.com/Data_values
 */

enum MINECRAFT_BLOCK {
    MINECRAFT_BLOCK_AIR                              =  0,
    MINECRAFT_BLOCK_STONE                            =  1,
    MINECRAFT_BLOCK_GRASS                            =  2,
    MINECRAFT_BLOCK_DIRT                             =  3,
    MINECRAFT_BLOCK_COBBLESTONE                      =  4,
    MINECRAFT_BLOCK_PLANKS                           =  5,
    MINECRAFT_BLOCK_SAPLING                          =  6,
    MINECRAFT_BLOCK_BEDROCK                          =  7,
    MINECRAFT_BLOCK_FLOWING_WATER                    =  8,
    MINECRAFT_BLOCK_WATER                            =  9,
    MINECRAFT_BLOCK_FLOWING_LAVA                     =  10,
    MINECRAFT_BLOCK_LAVA                             =  11,
    MINECRAFT_BLOCK_SAND                             =  12,
    MINECRAFT_BLOCK_GRAVEL                           =  13,
    MINECRAFT_BLOCK_GOLD_ORE                         =  14,
    MINECRAFT_BLOCK_IRON_ORE                         =  15,
    MINECRAFT_BLOCK_COAL_ORE                         =  16,
    MINECRAFT_BLOCK_LOG                              =  17,
    MINECRAFT_BLOCK_LEAVES                           =  18,
    MINECRAFT_BLOCK_SPONGE                           =  19,
    MINECRAFT_BLOCK_GLASS                            =  20,
    MINECRAFT_BLOCK_LAPIS_ORE                        =  21,
    MINECRAFT_BLOCK_LAPIS_BLOCK                      =  22,
    MINECRAFT_BLOCK_DISPENSER                        =  23,
    MINECRAFT_BLOCK_SANDSTONE                        =  24,
    MINECRAFT_BLOCK_NOTE_BLOCK                       =  25,
    MINECRAFT_BLOCK_BED                              =  26,
    MINECRAFT_BLOCK_GOLDEN_RAIL                      =  27,
    MINECRAFT_BLOCK_DETECTOR_RAIL                    =  28,
    MINECRAFT_BLOCK_STICKY_PISTON                    =  29,
    MINECRAFT_BLOCK_WEB                              =  30,
    MINECRAFT_BLOCK_TALLGRASS                        =  31,
    MINECRAFT_BLOCK_DEADBUSH                         =  32,
    MINECRAFT_BLOCK_PISTON                           =  33,
    MINECRAFT_BLOCK_PISTON_HEAD                      =  34,
    MINECRAFT_BLOCK_WOOL                             =  35,
    MINECRAFT_BLOCK_PISTON_EXTENSION                 =  36,
    MINECRAFT_BLOCK_YELLOW_FLOWER                    =  37,
    MINECRAFT_BLOCK_RED_FLOWER                       =  38,
    MINECRAFT_BLOCK_BROWN_MUSHROOM                   =  39,
    MINECRAFT_BLOCK_RED_MUSHROOM                     =  40,
    MINECRAFT_BLOCK_GOLD_BLOCK                       =  41,
    MINECRAFT_BLOCK_IRON_BLOCK                       =  42,
    MINECRAFT_BLOCK_DOUBLE_STONE_SLAB                =  43,
    MINECRAFT_BLOCK_STONE_SLAB                       =  44,
    MINECRAFT_BLOCK_BRICK_BLOCK                      =  45,
    MINECRAFT_BLOCK_TNT                              =  46,
    MINECRAFT_BLOCK_BOOKSHELF                        =  47,
    MINECRAFT_BLOCK_MOSSY_COBBLESTONE                =  48,
    MINECRAFT_BLOCK_OBSIDIAN                         =  49,
    MINECRAFT_BLOCK_TORCH                            =  50,
    MINECRAFT_BLOCK_FIRE                             =  51,
    MINECRAFT_BLOCK_MOB_SPAWNER                      =  52,
    MINECRAFT_BLOCK_OAK_STAIRS                       =  53,
    MINECRAFT_BLOCK_CHEST                            =  54,
    MINECRAFT_BLOCK_REDSTONE_WIRE                    =  55,
    MINECRAFT_BLOCK_DIAMOND_ORE                      =  56,
    MINECRAFT_BLOCK_DIAMOND_BLOCK                    =  57,
    MINECRAFT_BLOCK_CRAFTING_TABLE                   =  58,
    MINECRAFT_BLOCK_WHEAT                            =  59,
    MINECRAFT_BLOCK_FARMLAND                         =  60,
    MINECRAFT_BLOCK_FURNACE                          =  61,
    MINECRAFT_BLOCK_LIT_FURNACE                      =  62,
    MINECRAFT_BLOCK_STANDING_SIGN                    =  63,
    MINECRAFT_BLOCK_WOODEN_DOOR                      =  64,
    MINECRAFT_BLOCK_LADDER                           =  65,
    MINECRAFT_BLOCK_RAIL                             =  66,
    MINECRAFT_BLOCK_STONE_STAIRS                     =  67,
    MINECRAFT_BLOCK_WALL_SIGN                        =  68,
    MINECRAFT_BLOCK_LEVER                            =  69,
    MINECRAFT_BLOCK_STONE_PRESSURE_PLATE             =  70,
    MINECRAFT_BLOCK_IRON_DOOR                        =  71,
    MINECRAFT_BLOCK_WOODEN_PRESSURE_PLATE            =  72,
    MINECRAFT_BLOCK_REDSTONE_ORE                     =  73,
    MINECRAFT_BLOCK_LIT_REDSTONE_ORE                 =  74,
    MINECRAFT_BLOCK_UNLIT_REDSTONE_TORCH             =  75,
    MINECRAFT_BLOCK_REDSTONE_TORCH                   =  76,
    MINECRAFT_BLOCK_STONE_BUTTON                     =  77,
    MINECRAFT_BLOCK_SNOW_LAYER                       =  78,
    MINECRAFT_BLOCK_ICE                              =  79,
    MINECRAFT_BLOCK_SNOW                             =  80,
    MINECRAFT_BLOCK_CACTUS                           =  81,
    MINECRAFT_BLOCK_CLAY                             =  82,
    MINECRAFT_BLOCK_REEDS                            =  83,
    MINECRAFT_BLOCK_JUKEBOX                          =  84,
    MINECRAFT_BLOCK_FENCE                            =  85,
    MINECRAFT_BLOCK_PUMPKIN                          =  86,
    MINECRAFT_BLOCK_NETHERRACK                       =  87,
    MINECRAFT_BLOCK_SOUL_SAND                        =  88,
    MINECRAFT_BLOCK_GLOWSTONE                        =  89,
    MINECRAFT_BLOCK_PORTAL                           =  90,
    MINECRAFT_BLOCK_LIT_PUMPKIN                      =  91,
    MINECRAFT_BLOCK_CAKE                             =  92,
    MINECRAFT_BLOCK_UNPOWERED_REPEATER               =  93,
    MINECRAFT_BLOCK_POWERED_REPEATER                 =  94,
    MINECRAFT_BLOCK_STAINED_GLASS                    =  95,
    MINECRAFT_BLOCK_TRAPDOOR                         =  96,
    MINECRAFT_BLOCK_MONSTER_EGG                      =  97,
    MINECRAFT_BLOCK_STONE_BRICK                      =  98,
    MINECRAFT_BLOCK_BROWN_MUSHROOM_BLOCK             =  99,
    MINECRAFT_BLOCK_RED_MUSHROOM_BLOCK               =  100,
    MINECRAFT_BLOCK_IRON_BARS                        =  101,
    MINECRAFT_BLOCK_GLASS_PANE                       =  102,
    MINECRAFT_BLOCK_MELON_BLOCK                      =  103,
    MINECRAFT_BLOCK_PUMPKIN_STEM                     =  104,
    MINECRAFT_BLOCK_MELON_STEM                       =  105,
    MINECRAFT_BLOCK_VINE                             =  106,
    MINECRAFT_BLOCK_FENCE_GATE                       =  107,
    MINECRAFT_BLOCK_BRICK_STAIRS                     =  108,
    MINECRAFT_BLOCK_STONE_BRICK_STAIRS               =  109,
    MINECRAFT_BLOCK_MYCELIUM                         =  110,
    MINECRAFT_BLOCK_WATERLILY                        =  111,
    MINECRAFT_BLOCK_NETHER_BRICK                     =  112,
    MINECRAFT_BLOCK_NETHER_BRICK_FENCE               =  113,
    MINECRAFT_BLOCK_NETHER_BRICK_STAIRS              =  114,
    MINECRAFT_BLOCK_NETHER_WART                      =  115,
    MINECRAFT_BLOCK_ENCHANTING_TABLE                 =  116,
    MINECRAFT_BLOCK_BREWING_STAND                    =  117,
    MINECRAFT_BLOCK_CAULDRON                         =  118,
    MINECRAFT_BLOCK_END_PORTAL                       =  119,
    MINECRAFT_BLOCK_END_PORTAL_FRAME                 =  120,
    MINECRAFT_BLOCK_END_STONE                        =  121,
    MINECRAFT_BLOCK_DRAGON_EGG                       =  122,
    MINECRAFT_BLOCK_REDSTONE_LAMP                    =  123,
    MINECRAFT_BLOCK_LIT_REDSTONE_LAMP                =  124,
    MINECRAFT_BLOCK_DOUBLE_WOODEN_SLAB               =  125,
    MINECRAFT_BLOCK_WOODEN_SLAB                      =  126,
    MINECRAFT_BLOCK_COCOA                            =  127,
    MINECRAFT_BLOCK_SANDSTONE_STAIRS                 =  128,
    MINECRAFT_BLOCK_EMERALD_ORE                      =  129,
    MINECRAFT_BLOCK_ENDER_CHEST                      =  130,
    MINECRAFT_BLOCK_TRIPWIRE_HOOK                    =  131,
    MINECRAFT_BLOCK_TRIPWIRE                         =  132,
    MINECRAFT_BLOCK_EMERALD_BLOCK                    =  133,
    MINECRAFT_BLOCK_SPRUCE_STAIRS                    =  134,
    MINECRAFT_BLOCK_BIRCH_STAIRS                     =  135,
    MINECRAFT_BLOCK_JUNGLE_STAIRS                    =  136,
    MINECRAFT_BLOCK_COMMAND_BLOCK                    =  137,
    MINECRAFT_BLOCK_BEACON                           =  138,
    MINECRAFT_BLOCK_COBBLESTONE_WALL                 =  139,
    MINECRAFT_BLOCK_FLOWER_POT                       =  140,
    MINECRAFT_BLOCK_CARROTS                          =  141,
    MINECRAFT_BLOCK_POTATOES                         =  142,
    MINECRAFT_BLOCK_WOODEN_BUTTON                    =  143,
    MINECRAFT_BLOCK_SKULL                            =  144,
    MINECRAFT_BLOCK_ANVIL                            =  145,
    MINECRAFT_BLOCK_TRAPPED_CHEST                    =  146,
    MINECRAFT_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE    =  147,
    MINECRAFT_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE    =  148,
    MINECRAFT_BLOCK_UNPOWERED_COMPARATOR             =  149,
    MINECRAFT_BLOCK_POWERED_COMPARATOR               =  150,
    MINECRAFT_BLOCK_DAYLIGHT_DETECTOR                =  151,
    MINECRAFT_BLOCK_REDSTONE_BLOCK                   =  152,
    MINECRAFT_BLOCK_QUARTZ_ORE                       =  153,
    MINECRAFT_BLOCK_HOPPER                           =  154,
    MINECRAFT_BLOCK_QUARTZ_BLOCK                     =  155,
    MINECRAFT_BLOCK_QUARTZ_STAIRS                    =  156,
    MINECRAFT_BLOCK_ACTIVATOR_RAIL                   =  157,
    MINECRAFT_BLOCK_DROPPER                          =  158,
    MINECRAFT_BLOCK_STAINED_HARDENED_CLAY            =  159,
    MINECRAFT_BLOCK_STAINED_GLASS_PANE               =  160,
    MINECRAFT_BLOCK_LEAVES2                          =  161,
    MINECRAFT_BLOCK_LOG2                             =  162,
    MINECRAFT_BLOCK_ACACIA_STAIRS                    =  163,
    MINECRAFT_BLOCK_DARK_OAK_STAIRS                  =  164,
    MINECRAFT_BLOCK_SLIME                            =  165,
    MINECRAFT_BLOCK_BARRIER                          =  166,
    MINECRAFT_BLOCK_IRON_TRAPDOOR                    =  167,
    MINECRAFT_BLOCK_PRISMARINE                       =  168,
    MINECRAFT_BLOCK_SEA_LANTERN                      =  169,
    MINECRAFT_BLOCK_HAY_BLOCK                        =  170,
    MINECRAFT_BLOCK_CARPET                           =  171,
    MINECRAFT_BLOCK_HARDENED_CLAY                    =  172,
    MINECRAFT_BLOCK_COAL_BLOCK                       =  173,
    MINECRAFT_BLOCK_PACKED_ICE                       =  174,
    MINECRAFT_BLOCK_DOUBLE_PLANT                     =  175,
    MINECRAFT_BLOCK_STANDING_BANNER                  =  176,
    MINECRAFT_BLOCK_WALL_BANNER                      =  177,
    MINECRAFT_BLOCK_DAYLIGHT_DETECTOR_INVERTED       =  178,
    MINECRAFT_BLOCK_RED_SANDSTONE                    =  179,
    MINECRAFT_BLOCK_RED_SANDSTONE_STAIRS             =  180,
    MINECRAFT_BLOCK_DOUBLE_STONE_SLAB2               =  181,
    MINECRAFT_BLOCK_STONE_SLAB2                      =  182,
    MINECRAFT_BLOCK_SPRUCE_FENCE_GATE                =  183,
    MINECRAFT_BLOCK_BIRCH_FENCE_GATE                 =  184,
    MINECRAFT_BLOCK_JUNGLE_FENCE_GATE                =  185,
    MINECRAFT_BLOCK_DARK_OAK_FENCE_GATE              =  186,
    MINECRAFT_BLOCK_ACACIA_FENCE_GATE                =  187,
    MINECRAFT_BLOCK_SPRUCE_FENCE                     =  188,
    MINECRAFT_BLOCK_BIRCH_FENCE                      =  189,
    MINECRAFT_BLOCK_JUNGLE_FENCE                     =  190,
    MINECRAFT_BLOCK_DARK_OAK_FENCE                   =  191,
    MINECRAFT_BLOCK_ACACIA_FENCE                     =  192,
    MINECRAFT_BLOCK_SPRUCE_DOOR                      =  193,
    MINECRAFT_BLOCK_BIRCH_DOOR                       =  194,
    MINECRAFT_BLOCK_JUNGLE_DOOR                      =  195,
    MINECRAFT_BLOCK_ACACIA_DOOR                      =  196,
    MINECRAFT_BLOCK_DARK_OAK_DOOR                    =  197,
    MINECRAFT_BLOCK_END_ROD                          =  198,
    MINECRAFT_BLOCK_CHORUS_PLANT                     =  199,
    MINECRAFT_BLOCK_CHORUS_FLOWER                    =  200,
    MINECRAFT_BLOCK_PURPUR_BLOCK                     =  201,
    MINECRAFT_BLOCK_PURPUR_PILLAR                    =  202,
    MINECRAFT_BLOCK_PURPUR_STAIRS                    =  203,
    MINECRAFT_BLOCK_PURPUR_DOUBLE_SLAB               =  204,
    MINECRAFT_BLOCK_PURPUR_SLAB                      =  205,
    MINECRAFT_BLOCK_END_BRICKS                       =  206,
    MINECRAFT_BLOCK_BEETROOTS                        =  207,
    MINECRAFT_BLOCK_GRASS_PATH                       =  208,
    MINECRAFT_BLOCK_END_GATEWAY                      =  209,
    MINECRAFT_BLOCK_REPEATING_COMMAND_BLOCK          =  210,
    MINECRAFT_BLOCK_CHAIN_COMMAND_BLOCK              =  211,
    MINECRAFT_BLOCK_FROSTED_ICE                      =  212,
    MINECRAFT_BLOCK_MAGMA                            =  213,
    MINECRAFT_BLOCK_NETHER_WART_BLOCK                =  214,
    MINECRAFT_BLOCK_RED_NETHER_BRICK                 =  215,
    MINECRAFT_BLOCK_BONE_BLOCK                       =  216,
    MINECRAFT_BLOCK_STRUCTURE_VOID                   =  217,
    MINECRAFT_BLOCK_STRUCTURE_BLOCK                  =  255,
};































/* Item IDs */
#define MINECRAFT_ITEM_IRON_SHOVEL                       		256
#define MINECRAFT_ITEM_IRON_PICKAXE                      		257
#define MINECRAFT_ITEM_IRON_AXE                          		258
#define MINECRAFT_ITEM_FLINT_AND_STEEL                   		259
#define MINECRAFT_ITEM_APPLE                             		260
#define MINECRAFT_ITEM_BOW                               		261
#define MINECRAFT_ITEM_ARROW                             		262
#define MINECRAFT_ITEM_COAL                              		263
#define MINECRAFT_ITEM_DIAMOND                           		264
#define MINECRAFT_ITEM_IRON_INGOT                        		265
#define MINECRAFT_ITEM_GOLD_INGOT                        		266
#define MINECRAFT_ITEM_IRON_SWORD                        		267
#define MINECRAFT_ITEM_WOODEN_SWORD                      		268
#define MINECRAFT_ITEM_WOODEN_SHOVEL                     		269
#define MINECRAFT_ITEM_WOODEN_PICKAXE                    		270
#define MINECRAFT_ITEM_WOODEN_AXE                        		271
#define MINECRAFT_ITEM_STONE_SWORD                       		272
#define MINECRAFT_ITEM_STONE_SHOVEL                      		273
#define MINECRAFT_ITEM_STONE_PICKAXE                     		274
#define MINECRAFT_ITEM_STONE_AXE                         		275
#define MINECRAFT_ITEM_DIAMOND_SWORD                     		276
#define MINECRAFT_ITEM_DIAMOND_SHOVEL                    		277
#define MINECRAFT_ITEM_DIAMOND_PICKAXE                   		278
#define MINECRAFT_ITEM_DIAMOND_AXE                       		279
#define MINECRAFT_ITEM_STICK                             		280
#define MINECRAFT_ITEM_BOWL                              		281
#define MINECRAFT_ITEM_MUSHROOM_STEW                     		282
#define MINECRAFT_ITEM_GOLDEN_SWORD                      		283
#define MINECRAFT_ITEM_GOLDEN_SHOVEL                     		284
#define MINECRAFT_ITEM_GOLDEN_PICKAXE                    		285
#define MINECRAFT_ITEM_GOLDEN_AXE                        		286
#define MINECRAFT_ITEM_STRING                            		287
#define MINECRAFT_ITEM_FEATHER                           		288
#define MINECRAFT_ITEM_GUNPOWDER                         		289
#define MINECRAFT_ITEM_WOODEN_HOE                        		290
#define MINECRAFT_ITEM_STONE_HOE                         		291
#define MINECRAFT_ITEM_IRON_HOE                          		292
#define MINECRAFT_ITEM_DIAMOND_HOE                       		293
#define MINECRAFT_ITEM_GOLDEN_HOE                        		294
#define MINECRAFT_ITEM_WHEAT_SEEDS                       		295
#define MINECRAFT_ITEM_WHEAT                             		296
#define MINECRAFT_ITEM_BREAD                             		297
#define MINECRAFT_ITEM_LEATHER_HELMET                    		298
#define MINECRAFT_ITEM_LEATHER_CHESTPLATE                		299
#define MINECRAFT_ITEM_LEATHER_LEGGINGS                  		300
#define MINECRAFT_ITEM_LEATHER_BOOTS lEATHER             		301
#define MINECRAFT_ITEM_CHAINMAIL_HELMET                  		302
#define MINECRAFT_ITEM_CHAINMAIL_CHESTPLATE              		303
#define MINECRAFT_ITEM_CHAINMAIL_LEGGINGS                		304
#define MINECRAFT_ITEM_CHAINMAIL_BOOTS                   		305
#define MINECRAFT_ITEM_IRON_HELMET                       		306
#define MINECRAFT_ITEM_IRON_CHESTPLATE                   		307
#define MINECRAFT_ITEM_IRON_LEGGINGS                     		308
#define MINECRAFT_ITEM_IRON_BOOTS                        		309
#define MINECRAFT_ITEM_DIAMOND_HELMET                    		310
#define MINECRAFT_ITEM_DIAMOND_CHESTPLATE                		311
#define MINECRAFT_ITEM_DIAMOND_LEGGINGS                  		312
#define MINECRAFT_ITEM_DIAMOND_BOOTS                     		313
#define MINECRAFT_ITEM_GOLDEN_HELMET                     		314
#define MINECRAFT_ITEM_GOLDEN_CHESTPLATE                 		315
#define MINECRAFT_ITEM_GOLDEN_LEGGINGS                   		316
#define MINECRAFT_ITEM_GOLDEN_BOOTS                      		317
#define MINECRAFT_ITEM_FLINT                             		318
#define MINECRAFT_ITEM_PORKCHOP                          		319
#define MINECRAFT_ITEM_COOKED_PORKCHOP                   		320
#define MINECRAFT_ITEM_PAINTING                          		321
#define MINECRAFT_ITEM_GOLDEN_APPLE                      		322
#define MINECRAFT_ITEM_SIGN                              		323
#define MINECRAFT_ITEM_WOODEN_DOOR                       		324
#define MINECRAFT_ITEM_BUCKET                            		325
#define MINECRAFT_ITEM_WATER_BUCKET                      		326
#define MINECRAFT_ITEM_LAVA_BUCKET                       		327
#define MINECRAFT_ITEM_MINECART                          		328
#define MINECRAFT_ITEM_SADDLE                            		329
#define MINECRAFT_ITEM_IRON_DOOR                         		330
#define MINECRAFT_ITEM_REDSTONE                          		331
#define MINECRAFT_ITEM_SNOWBALL                          		332
#define MINECRAFT_ITEM_BOAT                              		333
#define MINECRAFT_ITEM_LEATHER                           		334
#define MINECRAFT_ITEM_MILK_BUCKET                       		335
#define MINECRAFT_ITEM_BRICK                             		336
#define MINECRAFT_ITEM_CLAY_BALL                         		337
#define MINECRAFT_ITEM_REEDS                             		338
#define MINECRAFT_ITEM_PAPER                             		339
#define MINECRAFT_ITEM_BOOK                              		340
#define MINECRAFT_ITEM_SLIME_BALL                        		341
#define MINECRAFT_ITEM_CHEST_MINECART                    		342
#define MINECRAFT_ITEM_FURNACE_MINECART                  		343
#define MINECRAFT_ITEM_EGG                               		344
#define MINECRAFT_ITEM_COMPASS                           		345
#define MINECRAFT_ITEM_FISHING_ROD                       		346
#define MINECRAFT_ITEM_CLOCK                             		347
#define MINECRAFT_ITEM_GLOWSTONE_DUST                    		348
#define MINECRAFT_ITEM_FISH                              		349
#define MINECRAFT_ITEM_COOKED_FISH                       		350
#define MINECRAFT_ITEM_DYE                               		351
#define MINECRAFT_ITEM_BONE                              		352
#define MINECRAFT_ITEM_SUGAR                             		353
#define MINECRAFT_ITEM_CAKE                              		354
#define MINECRAFT_ITEM_BED                               		355
#define MINECRAFT_ITEM_REPEATER                          		356
#define MINECRAFT_ITEM_COOKIE                            		357
#define MINECRAFT_ITEM_FILLED_MAP                        		358
#define MINECRAFT_ITEM_SHEARS                            		359
#define MINECRAFT_ITEM_MELON                             		360
#define MINECRAFT_ITEM_PUMPKIN_SEEDS                     		361
#define MINECRAFT_ITEM_MELON_SEEDS                       		362
#define MINECRAFT_ITEM_BEEF                              		363
#define MINECRAFT_ITEM_COOKED_BEEF                       		364
#define MINECRAFT_ITEM_CHICKEN                           		365
#define MINECRAFT_ITEM_COOKED_CHICKEN                    		366
#define MINECRAFT_ITEM_ROTTEN_FLESH                      		367
#define MINECRAFT_ITEM_ENDER_PEARL                       		368
#define MINECRAFT_ITEM_BLAZE_ROD                         		369
#define MINECRAFT_ITEM_GHAST_TEAR                        		370
#define MINECRAFT_ITEM_GOLD_NUGGET                       		371
#define MINECRAFT_ITEM_NETHER_WART                       		372
#define MINECRAFT_ITEM_POTION                            		373
#define MINECRAFT_ITEM_GLASS_BOTTLE                      		374
#define MINECRAFT_ITEM_SPIDER_EYE                        		375
#define MINECRAFT_ITEM_FERMENTED_SPIDER_EYE              		376
#define MINECRAFT_ITEM_BLAZE_POWDER                      		377
#define MINECRAFT_ITEM_MAGMA_CREAM                       		378
#define MINECRAFT_ITEM_BREWING_STAND                     		379
#define MINECRAFT_ITEM_CAULDRON                          		380
#define MINECRAFT_ITEM_ENDER_EYE                         		381
#define MINECRAFT_ITEM_SPECKLED_MELON                    		382
#define MINECRAFT_ITEM_SPAWN_EGG                         		383
#define MINECRAFT_ITEM_EXPERIENCE_BOTTLE                 		384
#define MINECRAFT_ITEM_FIRE_CHARGE                       		385
#define MINECRAFT_ITEM_WRITABLE_BOOK                     		386
#define MINECRAFT_ITEM_WRITTEN_BOOK                      		387
#define MINECRAFT_ITEM_EMERALD                           		388
#define MINECRAFT_ITEM_ITEM_FRAME                        		389
#define MINECRAFT_ITEM_FLOWER_POT                        		390
#define MINECRAFT_ITEM_CARROT                            		391
#define MINECRAFT_ITEM_POTATO                            		392
#define MINECRAFT_ITEM_BAKED_POTATO                      		393
#define MINECRAFT_ITEM_POISONOUS_POTATO                  		394
#define MINECRAFT_ITEM_MAP                               		395
#define MINECRAFT_ITEM_GOLDEN_CARROT                     		396
#define MINECRAFT_ITEM_SKULL                             		397
#define MINECRAFT_ITEM_CARROT_ON_A_STICK                 		398
#define MINECRAFT_ITEM_NETHER_STAR                       		399
#define MINECRAFT_ITEM_PUMPKIN_PIE                       		400
#define MINECRAFT_ITEM_FIREWORKS                         		401
#define MINECRAFT_ITEM_FIREWORK_CHARGE                   		402
#define MINECRAFT_ITEM_ENCHANTED_BOOK                    		403
#define MINECRAFT_ITEM_COMPARATOR                        		404
#define MINECRAFT_ITEM_NETHERBRICK                       		405
#define MINECRAFT_ITEM_QUARTZ                            		406
#define MINECRAFT_ITEM_TNT_MINECART                      		407
#define MINECRAFT_ITEM_HOPPER_MINECART                   		408
#define MINECRAFT_ITEM_PRISMARINE_SHARD                  		409
#define MINECRAFT_ITEM_PRISMARINE_CRYSTALS               		410
#define MINECRAFT_ITEM_RABBIT                            		411
#define MINECRAFT_ITEM_COOKED_RABBIT                     		412
#define MINECRAFT_ITEM_RABBIT_STEW                       		413
#define MINECRAFT_ITEM_RABBIT_FOOT                       		414
#define MINECRAFT_ITEM_RABBIT_HIDE                       		415
#define MINECRAFT_ITEM_ARMOR_STAND                       		416
#define MINECRAFT_ITEM_IRON_HORSE_ARMOR                  		417
#define MINECRAFT_ITEM_GOLDEN_HORSE_ARMOR                		418
#define MINECRAFT_ITEM_DIAMOND_HORSE_ARMOR               		419
#define MINECRAFT_ITEM_LEAD                              		420
#define MINECRAFT_ITEM_NAME_TAG                          		421
#define MINECRAFT_ITEM_COMMAND_BLOCK_MINECART            		422
#define MINECRAFT_ITEM_MUTTON                            		423
#define MINECRAFT_ITEM_COOKED_MUTTON                     		424
#define MINECRAFT_ITEM_BANNER                            		425
#define MINECRAFT_ITEM_END_CRYSTAL                       		426
#define MINECRAFT_ITEM_SPRUCE_DOOR                       		427
#define MINECRAFT_ITEM_BIRCH_DOOR                        		428
#define MINECRAFT_ITEM_JUNGLE_DOOR                       		429
#define MINECRAFT_ITEM_ACACIA_DOOR                       		430
#define MINECRAFT_ITEM_DARK_OAK_DOOR                     		431
#define MINECRAFT_ITEM_CHORUS_FRUIT                      		432
#define MINECRAFT_ITEM_CHORUS_FRUIT_POPPED               		433
#define MINECRAFT_ITEM_BEETROOT                          		434
#define MINECRAFT_ITEM_BEETROOT_SEEDS                    		435
#define MINECRAFT_ITEM_BEETROOT_SOUP                     		436
#define MINECRAFT_ITEM_DRAGON_BREATH                     		437
#define MINECRAFT_ITEM_SPLASH_POTION                     		438
#define MINECRAFT_ITEM_SPECTRAL_ARROW                    		439
#define MINECRAFT_ITEM_TIPPED_ARROW                      		440
#define MINECRAFT_ITEM_LINGERING_POTION                  		441
#define MINECRAFT_ITEM_SHIELD                            		442
#define MINECRAFT_ITEM_ELYTRA                            		443
#define MINECRAFT_ITEM_SPRUCE_BOAT                       		444
#define MINECRAFT_ITEM_BIRCH_BOAT                        		445
#define MINECRAFT_ITEM_JUNGLE_BOAT                       		446
#define MINECRAFT_ITEM_ACACIA_BOAT                       		447
#define MINECRAFT_ITEM_DARK_OAK_BOAT                     		448

/* Record Values */
#define MINECRAFT_ITEM_RECORD_13                         		2256
#define MINECRAFT_ITEM_RECORD_CAT                        		2257
#define MINECRAFT_ITEM_RECORD_BLOCKS                     		2258
#define MINECRAFT_ITEM_RECORD_CHIRP                      		2259
#define MINECRAFT_ITEM_RECORD_FAR                        		2260
#define MINECRAFT_ITEM_RECORD_MALL                       		2261
#define MINECRAFT_ITEM_RECORD_MELLOHI                    		2262
#define MINECRAFT_ITEM_RECORD_STAL                       		2263
#define MINECRAFT_ITEM_RECORD_STRAD                      		2264
#define MINECRAFT_ITEM_RECORD_WARD                       		2265
#define MINECRAFT_ITEM_RECORD_11                         		2266
#define MINECRAFT_ITEM_RECORD_WAIT                       		2267

#define MINECRAFT_ENTITY_ITEM                              		1               
#define MINECRAFT_ENTITY_XP_ORB                            		2                       
#define MINECRAFT_ENTITY_AREA_EFFECT_CLOUD                 		3                       
#define MINECRAFT_ENTITY_LEASH_KNOT                        		8                       
#define MINECRAFT_ENTITY_PAINTING                          		9                       
#define MINECRAFT_ENTITY_ITEM_FRAME                        		18                      
#define MINECRAFT_ENTITY_ARMOR_STAND                       		30                      
#define MINECRAFT_ENTITY_ENDER_CRYSTAL                     		200                     
#define MINECRAFT_ENTITY_THROWN_EGG                        		7                       
#define MINECRAFT_ENTITY_ARROW                             		10                      
#define MINECRAFT_ENTITY_SNOWBALL                          		11                      
#define MINECRAFT_ENTITY_FIREBALL                          		12                      
#define MINECRAFT_ENTITY_SMALL_FIREBALL                    		13                      
#define MINECRAFT_ENTITY_THROWN_ENDERPEARL                 		14                      
#define MINECRAFT_ENTITY_EYE_OF_ENDER_SIGNAL               		15                      
#define MINECRAFT_ENTITY_THROWN_POTION                     		16                      
#define MINECRAFT_ENTITY_THROWN_EXP_BOTTLE                 		17                      
#define MINECRAFT_ENTITY_WITHER_SKULL                      		19                      
#define MINECRAFT_ENTITY_FIREWORKS_ROCKET_ENTITY           		22                      
#define MINECRAFT_ENTITY_SPECTRAL_ARROW                    		24                      
#define MINECRAFT_ENTITY_SHULKER_BULLET                    		25                      
#define MINECRAFT_ENTITY_DRAGON_FIREBALL                   		26                      
#define MINECRAFT_ENTITY_PRIMED_TNT                        		20                      
#define MINECRAFT_ENTITY_FALLING_SAND                      		21                      
#define MINECRAFT_ENTITY_MINECART_COMMAND_BLOCK            		40                      
#define MINECRAFT_ENTITY_BOAT                              		41                      
#define MINECRAFT_ENTITY_MINECART_RIDEABLE                 		42                      
#define MINECRAFT_ENTITY_MINECART_CHEST                    		43                      
#define MINECRAFT_ENTITY_MINECART_FURNACE                  		44                      
#define MINECRAFT_ENTITY_MINECART_TNT                      		45                      
#define MINECRAFT_ENTITY_MINECART_HOPPER                   		46                      
#define MINECRAFT_ENTITY_MINECART_SPAWNER                  		47                      
#define MINECRAFT_ENTITY_MOB                               		48                      
#define MINECRAFT_ENTITY_MONSTER                           		49                         

/* Hostile Mobs */
#define MINECRAFT_ENTITY_CREEPER                           		50
#define MINECRAFT_ENTITY_SKELETON                          		50  /*  51  */
#define MINECRAFT_ENTITY_WITHER_SKELETON                   		51  /*   5  */
#define MINECRAFT_ENTITY_STRAY                             		51  /*   6  */
#define MINECRAFT_ENTITY_SPIDER                            		52
#define MINECRAFT_ENTITY_GIANT                             		53
#define MINECRAFT_ENTITY_ZOMBIE                            		53  /*  54  */
#define MINECRAFT_ENTITY_ZOMBIE_VILLAGER                   		54  /*  27  */
#define MINECRAFT_ENTITY_HUSK                              		54  /*  23  */
#define MINECRAFT_ENTITY_SLIME                             		55
#define MINECRAFT_ENTITY_GHAST                             		56
#define MINECRAFT_ENTITY_ZOMBIE_PIGMAN                     		57
#define MINECRAFT_ENTITY_ENDERMAN                          		58
#define MINECRAFT_ENTITY_CAVE_SPIDER                       		59
#define MINECRAFT_ENTITY_SILVERFISH                        		60
#define MINECRAFT_ENTITY_BLAZE                             		61
#define MINECRAFT_ENTITY_MAGMA_CUBE                        		62
#define MINECRAFT_ENTITY_ENDER_DRAGON                      		63
#define MINECRAFT_ENTITY_WITHER                            		64
#define MINECRAFT_ENTITY_WITCH                             		66
#define MINECRAFT_ENTITY_ENDERMITE                         		67
#define MINECRAFT_ENTITY_GUARDIAN                          		67  /*  68  */
#define MINECRAFT_ENTITY_ELDER_GUARDIAN                    		68
#define MINECRAFT_ENTITY_SHULKER                           		69

/* Passive Mobs */
#define MINECRAFT_ENTITY_BAT                               		65
#define MINECRAFT_ENTITY_PIG                               		90
#define MINECRAFT_ENTITY_SHEEP                             		91
#define MINECRAFT_ENTITY_COW                               		92
#define MINECRAFT_ENTITY_CHICKEN                           		93
#define MINECRAFT_ENTITY_SQUID                             		94
#define MINECRAFT_ENTITY_WOLF                              		95
#define MINECRAFT_ENTITY_MOOSHROOM                         		96
#define MINECRAFT_ENTITY_SNOWMAN                           		97
#define MINECRAFT_ENTITY_OCELOT                            		98
#define MINECRAFT_ENTITY_IRON_GOLEM                        		99
#define MINECRAFT_ENTITY_HORSE                             		99  /*  100 */
#define MINECRAFT_ENTITY_DONKEY                            		99  /*  31  */
#define MINECRAFT_ENTITY_MULE                              		100 /*  32  */
#define MINECRAFT_ENTITY_ZOMBIE_HORSE                      		100 /*  29  */
#define MINECRAFT_ENTITY_SKELETON_HORSE                    		100 /*  28  */
#define MINECRAFT_ENTITY_RABBIT                            		101
#define MINECRAFT_ENTITY_POLAR_BEAR                        		102
#define MINECRAFT_ENTITY_VILLAGER                          		120

/* Enchantments */
#define MINECRAFT_ENCHANTMENT_PROTECTION                        		0
#define MINECRAFT_ENCHANTMENT_FIRE_PROTECTION                   		1
#define MINECRAFT_ENCHANTMENT_FEATHER_FALLING                   		2
#define MINECRAFT_ENCHANTMENT_BLAST_PROTECTION                  		3
#define MINECRAFT_ENCHANTMENT_PROJECTILE_PROTECTION             		4
#define MINECRAFT_ENCHANTMENT_RESPIRATION                       		5
#define MINECRAFT_ENCHANTMENT_AQUA_AFFINITY                     		6
#define MINECRAFT_ENCHANTMENT_THORNS                            		7
#define MINECRAFT_ENCHANTMENT_DEPTH_STRIDER                     		8
#define MINECRAFT_ENCHANTMENT_FROST_WALKER                      		9
#define MINECRAFT_ENCHANTMENT_SHARPNESS                         		16
#define MINECRAFT_ENCHANTMENT_SMITE                             		17
#define MINECRAFT_ENCHANTMENT_BANE_OF_ARTHROPODS                		18
#define MINECFAFT_ENCHANTMENT_KNOCKBACK                         		19
#define MINECRAFT_ENCHANTMENT_FIRE_ASPECT                       		20
#define MINECRAFT_ENCHANTMENT_LOOTING                           		21
#define MINECRAFT_ENCHANTMENT_EFFICIENCY                        		32
#define MINECRAFT_ENCHANTMENT_SILK_TOUCH                        		33
#define MINECRAFT_ENCHANTMENT_UNBREAKING                        		34
#define MINECRAFT_ENCHANTMENT_FORTUNE                           		35
#define MINECRAFT_ENCHANTMENT_POWER                             		48
#define MINECRAFT_ENCHANTMENT_PUNCH                             		49
#define MINECRAFT_ENCHANTMENT_FLAME                             		50
#define MINECRAFT_ENCHANTMENT_INFINITY                          		51
#define MINECRAFT_ENCHANTMENT_LUCK_OF_THE_SEA                   		61
#define MINECRAFT_ENCHANTMENT_LURE                              		62
#define MINECRAFT_ENCHANTMENT_MENDING                           		70

/* Status Effects*/
#define MINECRAFT_EFFECT_SPEED										1 
#define MINECRAFT_EFFECT_SLOWNESS                          		2 
#define MINECRAFT_EFFECT_HASTE                             		3 
#define MINECRAFT_EFFECT_MINING_FATIGUE                    		4 
#define MINECRAFT_EFFECT_STRENGTH                          		5 
#define MINECRAFT_EFFECT_INSTANT_HEALTH                    		6 
#define MINECRAFT_EFFECT_INSTANT_DAMAGE                    		7 
#define MINECRAFT_EFFECT_JUMP_BOOST                        		8 
#define MINECRAFT_EFFECT_NAUSEA                            		9 
#define MINECRAFT_EFFECT_REGENERATION                      		10
#define MINECRAFT_EFFECT_RESISTANCE                        		11
#define MINECRAFT_EFFECT_FIRE_RESISTANCE                   		12
#define MINECRAFT_EFFECT_WATER_BREATHING                   		13
#define MINECRAFT_EFFECT_INVISIBILITY                      		14
#define MINECRAFT_EFFECT_BLINDNESS                         		15
#define MINECRAFT_EFFECT_NIGHT_VISION                      		16
#define MINECRAFT_EFFECT_HUNGER                            		17
#define MINECRAFT_EFFECT_WEAKNESS                          		18
#define MINECRAFT_EFFECT_POISON                            		19
#define MINECRAFT_EFFECT_WITHER                            		20
#define MINECRAFT_EFFECT_HEALTH_BOOST                      		21
#define MINECRAFT_EFFECT_ABSORPTION                        		22
#define MINECRAFT_EFFECT_SATURATION                        		23
#define MINECRAFT_EFFECT_GLOWING                           		24
#define MINECRAFT_EFFECT_LEVITATION                        		25
#define MINECRAFT_EFFECT_LUCK                              		26
#define MINECRAFT_EFFECT_UNLUCK                            		27
