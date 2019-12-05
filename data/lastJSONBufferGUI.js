{
	"templates": {
		"dragPad": {
			"type":5
		},
		"menuBar": {
			"type":4,
			"ss":"headerSS",
			"alignX":1,
			"fillRatioX":1
		},
		"sectionBar": {
			"type":0,
			"ss":"sectionSS",
			"alignX":1,
			"fillRatioX":1
		},
		"spacerBar": {
			"type":0,
			"ss":"spacerSS",
			"alignX":1,
			"fillRatioX":1,
			"label":" "
		},
		"cont": {
			"type":0
		},
		"rootCont": {
			"type":0,
			"hoverType":4
		},
		"fillToggle": {
			"type":1,
			"fillRatioX":1,
			"divisions":1
		},
		"fillSlider": {
			"type":1,
			"fillRatioX":1
		},
		"fillSliderCenter": {
			"type":1,
			"fillRatioX":1,
			"alignX":1
		},
		"fillSliderHSV": {
			"type":1,
			"fillRatioX":1,
			"matCode":1
		},
		"fillContMat": {
			"type":0,
			"fillRatioX":1,
			"matCode":2
		},
		"fillLabel": {
			"type":0,
			"label":" ",
			"fillRatioX":1
		},
		"fillLabelCenter": {
			"type":0,
			"label":" ",
			"fillRatioX":1,
			"alignX":1
		},
		"fillButton": {
			"type":2,
			"fillRatioX":1
		},
		"fillButtonCenter": {
			"type":2,
			"fillRatioX":1,
			"alignX":1
		},
		"statSlider": {
			"type":1,
			"label":" ",
			"fillRatioX":1,
			"divisions":10,
			"alignX":1
		},
		"tooltip": {
			"type":0,
			"hoverType":1,
			"layer":3,
			"hasBackground":0
		},
		"tooltipValue": {
			"type":0,
			"hoverType":3,
			"layer":3,
			"hasBackground":0
		},
		"popout": {
			"type":0,
			"hoverType":2,
			"layer":1,
			"hasBackground":0
		},
		"toggle": {
			"type":1,
			"divisions":1
		},
		"slider": {
			"type":1
		},
		"button": {
			"type":2
		},
		"radio": {
			"type":3
		},
		"radioFillCenter": {
			"type":3,
			"fillRatioX":1,
			"alignX":1
		},
		"checkFillCenter": {
			"type":6,
			"fillRatioX":1,
			"alignX":1
		},
		"radioFillCenterColor": {
			"type":3,
			"fillRatioX":1,
			"alignX":1,
			"matCode":1
		}
	},
	
	"sounds": {
		"mouseDown":{"name":"castinet0","vol":0.5},
		"mouseUp":{"name":"castinet0","vol":1.0},
		"mouseOver":{"name":"claves0","vol":0.6},
		"mouseOut":{"name":"","vol":1.0},
		"showGUI":{"name":"woodblock0","vol":1.0},
		"hideGUI":{"name":"woodblock0","vol":1.0}
	},
	
	
	"comment_1":"Max of 256 materials",
	
	"materialDefs": {
		"none":{"class":"none"},
	
		"Copper":{"class":"Ornate"},
		"Silver":{"class":"Ornate"},
		"Gold":{"class":"Ornate"},
		"Platinum":{"class":"Ornate"},
		
		
		"Onyx":{"class":"Gemstone"},
		"Amethyst":{"class":"Gemstone"},
		"Topaz":{"class":"Gemstone"},
		"Sapphire":{"class":"Gemstone"},
		"Emerald":{"class":"Gemstone"},
		"Ruby":{"class":"Gemstone"},
		"Diamond":{"class":"Gemstone"},
		
		"Sandstone":{"class":"Stone"},
		"Marble":{"class":"Stone"},
		"Granite":{"class":"Stone"},
	
		"Bronze":{"class":"Metal"},
		"Iron":{"class":"Metal"},
		"Steel":{"class":"Metal"},
		
		"Oak":{"class":"Wood"},
		"Redwood":{"class":"Wood"},
		"Yew":{"class":"Wood"},
		
		"Cotton":{"class":"Cloth"},
		"Linen":{"class":"Cloth"},
		"Silk":{"class":"Cloth"},
		
		"Leather":{"class":"Hide"},
		"Dragon Scale":{"class":"Hide"}
		
	},
	
	"itemDefs": {
		"none":{
			"iconNum":1367,
			"class":"none"
		},
		"Helm":{
			"iconNum":271,
			"class":"Helmet"
		},
		"Pendant":{
			"iconNum":20,
			"class":"Necklace"
		},
		"Cloak":{
			"iconNum":54,
			"class":"Cloak"
		},
		"Plated Mail":{
			"iconNum":126,
			"class":"Armor"
		},
		"Ring":{
			"iconNum":933,
			"class":"Gloves"
		},
		"Long Sword":{
			"iconNum":1158,
			"class":"Primary"
		},
		"Kite Shield":{
			"iconNum":975,
			"class":"Secondary"
		},
		"Boots":{
			"iconNum":130,
			"class":"Boots"
		},
		
		
		
		"Bow":{
			"iconNum":1274,
			"class":"Primary"
		},
		"Sceptre":{
			"iconNum":1197,
			"class":"Primary"
		},
		"Axe":{
			"iconNum":1329,
			"class":"Primary"
		},
		"Spellbook":{
			"iconNum":782,
			"class":"Primary"
		},
		"Hammer":{
			"iconNum":1180,
			"class":"Primary"
		},
		"Spear":{
			"iconNum":1342,
			"class":"Primary"
		},
		
		
		
		
		"Green Potion":{
			"iconNum":555,
			"class":"Consumable"
		},
		"Watermelon":{
			"iconNum":239,
			"class":"Consumable"
		},
		"Banana":{
			"iconNum":237,
			"class":"Consumable"
		},
		"Cheese":{
			"iconNum":256,
			"class":"Consumable"
		},
		"Pie":{
			"iconNum":251,
			"class":"Consumable"
		},
		"Mutton":{
			"iconNum":257,
			"class":"Consumable"
		}
		
	},
	
	"inventoryData":{
		"items":[
			{"eq":1, "quant":"1", "name":"Helm", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Pendant", "mat":"Ruby"},
			{"eq":1, "quant":"1", "name":"Cloak", "mat":"Silk"},
			{"eq":1, "quant":"1", "name":"Plated Mail", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Ring", "mat":"Ruby"},
			{"eq":1, "quant":"1", "name":"Long Sword", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Kite Shield", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Boots", "mat":"Leather"},
			
			{"eq":0, "quant":"1", "name":"Bow", "mat":"Yew"},
			{"eq":0, "quant":"1", "name":"Sceptre", "mat":"Iron"},
			{"eq":0, "quant":"1", "name":"Axe", "mat":"Iron"},
			{"eq":0, "quant":"1", "name":"Spellbook", "mat":"Leather"},
			{"eq":0, "quant":"1", "name":"Hammer", "mat":"Iron"},
			{"eq":0, "quant":"1", "name":"Spear", "mat":"Iron"},
			{"eq":0, "quant":"1", "name":"Green Potion", "mat":"none"},
			{"eq":0, "quant":"1", "name":"Watermelon", "mat":"none"},
			{"eq":0, "quant":"1", "name":"Banana", "mat":"none"},
			{"eq":0, "quant":"1", "name":"Cheese", "mat":"none"},
			{"eq":0, "quant":"1", "name":"Pie", "mat":"none"},
			{"eq":0, "quant":"1", "name":"Mutton", "mat":"none"}
		]
	},
	
	
	
	
	
	
	
	
	
	"baseGUI":{
		"template":"rootCont",
		"hasBackground":0,
		"floatingChildren":[
			
			
			
			
			
			
			
			
			
			
			{
				"uid":"guiHandles.E_FM_FIELDMENU",
				"template":"rootCont",
				"hasBackground":0,
				"alignX":1,
				"children":[
					{
						"template":"cont",
						"fillDir":1,
						"children": [
						
							
							{
								"template":"cont",
								"fillDir":1,
								"fillRatioX":1,
								"children":[
									
									{
										"template":"fillLabel",
										"uid":"fieldMenu.field"
									},
									{
										"template":"cont",
										"fillDir":0,
										"fillRatioX":1,
										"children":[
											{
												"ss":"greenSS",
												"template":"fillButton",
												"label":"Ok",
												"uid":"fieldMenu.ok"
												
											},
											{
												"ss":"redSS",
												"template":"fillButton",
												"label":"Cancel",
												"uid":"fieldMenu.cancel"
												
											}
										]
									}
									
									
									
									
								]
							}
						]
					}
				]
			},

			
			{
				"uid":"guiHandles.E_FM_DDMENU",
				"template":"rootCont",
				"hasBackground":0,
				"children":[
					{
						"template":"cont",
						"fillDir":1,
						"children": [
						
							{
								"template":"menuBar",
								"uid":"ddMenu.menuBar",
								"label":"Context Menu"
							},
							{
								"template":"cont",
								"fillDir":1,
								"fillRatioX":1,
								"children":[
									
									
									
									{
										"template":"fillButtonCenter",
										"label":"Trade With",
										"uid":"ddMenu.tradeWith"
									},
									{
										"template":"fillButtonCenter",
										"label":"Pickpocket",
										"uid":"ddMenu.pickPocket"
									},
									
									{
										"template":"radioFillCenter",
										"label":"Engage With...",
										"uid":"ddMenu.engageWith",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
												
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"children":[
																{
																	"template":"fillButtonCenter",
																	"label":"Shake Hands",
																	"uid":"ddMenu.engageWith.shakeHands"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Hug",
																	"uid":"ddMenu.engageWith.hug"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Kiss",
																	"uid":"ddMenu.engageWith.kiss"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Grope",
																	"uid":"ddMenu.engageWith.grope"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Slap",
																	"uid":"ddMenu.engageWith.slap"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Detain",
																	"uid":"ddMenu.engageWith.detain"
																}
																
																
															]
														}
													]
												}
												
											]
										}]
									},
									
									
									
									{
										"template":"radioFillCenter",
										"label":"Talk To...",
										"uid":"ddMenu.talkTo",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
												
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"children":[
																{
																	"template":"fillButtonCenter",
																	"label":"Compliment",
																	"uid":"ddMenu.talkTo.compliment"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Insult",
																	"uid":"ddMenu.talkTo.insult"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Joke With",
																	"uid":"ddMenu.talkTo.jokeWith"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Flirt",
																	"uid":"ddMenu.talkTo.flirt"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Seduce",
																	"uid":"ddMenu.talkTo.seduce"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Ask About",
																	"uid":"ddMenu.talkTo.askAbout"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Tell About",
																	"uid":"ddMenu.talkTo.tellAbout"
																}
																
															]
														}
													]
												}
												
											]
										}]
									},
									
									{
										"template":"radioFillCenter",
										"label":"Attack...",
										"uid":"ddMenu.attack",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
												
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"children":[
																{
																	"template":"fillButtonCenter",
																	"label":"Slash",
																	"uid":"ddMenu.attack.slash"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Stab",
																	"uid":"ddMenu.attack.stab"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Hack",
																	"uid":"ddMenu.attack.hack"
																}
																
															]
														}
													]
												}
												
											]
										}]
									},
									
									
									
									
									
									
									{
										"template":"radioFillCenter",
										"label":"Debug: Place Entity",
										"uid":"ddMenu.placeEntity",
										"ss":"redSS",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
												
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"children":[
																{
																	"template":"fillButtonCenter",
																	"label":"NPC",
																	"uid":"ddMenu.placeEntity.npc"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Sword",
																	"uid":"ddMenu.placeEntity.sword"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Axe",
																	"uid":"ddMenu.placeEntity.axe"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Mace",
																	"uid":"ddMenu.placeEntity.mace"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Hammer",
																	"uid":"ddMenu.placeEntity.hammer"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Staff",
																	"uid":"ddMenu.placeEntity.staff"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Spear",
																	"uid":"ddMenu.placeEntity.spear"
																},
																{
																	"template":"fillButtonCenter",
																	"label":"Building",
																	"uid":"ddMenu.placeEntity.building"
																}
															]
														}
													]
												}
												
											]
										}]
									},
									{
										"template":"fillButtonCenter",
										"label":"Debug: Remove Entity",
										"ss":"redSS",
										"uid":"ddMenu.removeEntity"
										
									},
									{
										"template":"radioFillCenter",
										"label":"Debug: Edit Entity",
										"ss":"redSS",
										"uid":"ddMenu.editEntity"
										
									},
									{
										"template":"radioFillCenter",
										"label":"Debug: Change Material",
										"ss":"redSS",
										"uid":"ddMenu.changeMaterial",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"menuBar",
															"label":"Materials"
															
														},
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"maxDimY": 512,
															"dataFile":"materials.js",
															"dataSource":"materials",
															"childType":"E_GCT_GENERIC",
															"children": [],
															"childTemplate":{
																"template":"radioFillCenter",
																"label":"",
																"uid":"#materialPicker"
															}
														}
													]
												}
											]
										}]
									}
									
									
									
									
								]
							}
						]
					}
				]
			},
			
			
			
			{
				"uid":"guiHandles.E_FM_CONTMENU",
				"template":"rootCont",
				"hasBackground":0,
				"children":[
					
					{
						"template":"cont",
						"fillDir":1,
						"children": [
							{
								"template":"menuBar",
								"label":"Open Container(s)"
							},
							
							{
								"template":"cont",
								"fillRatioX":1,
								"fillDir":0,
								
								"uid":"objectContainer",
								"dataFile":"E_SDT_OBJECTDATA",
								"dataSource":"objects",
								"childType":"E_GCT_CONTAINER_PARENT",
								"children":[],
								
								"childTemplate": {
									"template":"cont",
									"fillRatioX":1,
									"fillDir":1,
									
									"children": [
											{
												"template":"fillButton",
												"label":"Close",
												"alignX":1,
												"uid":"#contMenu.close",
												"ss":"redSS"
											},
											{
												"template":"cont",
												"fillDir":1,
												"fillRatioX":1,
												"maxDimY": 512,
												"minDimY": 32,
												
												
												"objectId":0,
												"uid":"#contItemParent",
												"interactive":1,
												
												"dataFile":"E_SDT_OBJECTDATA",
												"dataSource":"",
												"childType":"E_GCT_CONTAINER",
												"children":[],
												
												"childTemplate":{
													"template":"radioFillCenter",
													"label":"",
													"objectId":0,
													"objectType":0,
													"uid":"#contItem"
												}
										}
									]
								}
							}
						]
					}
					
					
					
					
				]
			},
			
			
			
			
			{
				"uid":"guiHandles.E_FM_HUDMENU",
				"template":"rootCont",
				"hasBackground":0,
				"children":[
					
					{
						"template":"cont",
						"fillDir":1,
						"uid":"hudMenu.hudContainer",
						"children": [
						
						
							{
								"template":"cont",
								"fillRatioX":1,
								"fillDir":0,
								"children":[
									{
										"template":"menuBar",
										"label":"HUD",
										"fillRatioX":5
									},
									{
										"template":"fillButton",
										"label":"X",
										"fillRatioX":1,
										"alignX":1,
										"uid":"hudMenu.close",
										"ss":"redSS"
									}
								]
							},
							
							{
								"template":"cont",
								"fillRatioX":1,
								"fillDir":0,						
								
								"children":[
									
									{
										"template":"cont",
										"fillRatioX":1,
										"fillDir":1,
										"children":[
											{
												"template":"sectionBar",
												"label":"Status"
											},
											
											
											{
												"template":"spacerBar"
											},
											
											{
												"template":"cont",
												"fillDir":1,
												"fillRatioX":1,
												"maxDimY": 512,
												"minDimY": 32,
												
												"uid":"hudMenu.statContainer",
												
												"dataFile":"E_SDT_STATUSDATA",
												"dataSource":"status",
												"childType":"E_GCT_AUTOMATIC",
												"children":[],
												
												"childTemplate":{
													"template":"fillLabelCenter",
													"label":"",
													"value":0,
													"divisions":10,
													"uid":"#hudMenu.status"
												}
											},

											{
												"template":"spacerBar"
											}



											
											
										]
									}
																
								]
								
							}
						]
					}
					
					
					
					
				]
			},
			
			
			
			{
				"uid":"guiHandles.E_FM_STATMENU",
				"template":"rootCont",
				"hasBackground":0,
				"children":[
					
					{
						"template":"cont",
						"fillDir":1,
						"uid":"statMenu.statContainer",
						"children": [
						
						
							{
								"template":"cont",
								"fillRatioX":1,
								"fillDir":0,
								"children":[
									{
										"template":"menuBar",
										"label":"Character",
										"fillRatioX":5
									},
									{
										"template":"fillButton",
										"label":"X",
										"fillRatioX":1,
										"alignX":1,
										"uid":"statMenu.close",
										"ss":"redSS"
									}
								]	
							},
							
							{
								"template":"cont",
								"fillRatioX":1,
								"fillDir":0,						
								
								"children":[
									{
										"template":"cont",
										"fillRatioX":1,
										"fillDir":1,
										"minDimX": 250,
										"children":[
											{
												"template":"sectionBar",
												"label":"Stats"
											},
											
											{
												"template":"spacerBar"
											},
											
											
											
											{
												"template":"cont",
												"fillDir":1,
												"fillRatioX":1,
												"maxDimY": 512,
												"minDimY": 32,
												
												"uid":"statMenu.statParent",
												"dataFile":"E_SDT_STATDATA",
												"dataSource":"stats",
												"childType":"E_GCT_AUTOMATIC",
												"children":[],
												
												"childTemplate":{
													"template":"statSlider",
													"label":"",
													"value":0,
													"divisions":10,
													"uid":"#statMenu.stat"
												}
											},
											
											{
												"template":"spacerBar"
											},
											
											{
												"template":"fillLabelCenter",
												"label":"Available Points",
												"value":0,
												"divisions":"@@E_JP_STATMENUAVAILDIV@@",
												"uid":"statMenu.availPoints"
											},
											
											
											{
												"template":"cont",
												"fillDir":0,
												"fillRatioX":1,
												"children":[
													{
														"template":"fillButtonCenter",
														"label":"Apply",
														"uid":"statMenu.apply"
													},
													{
														"template":"fillButtonCenter",
														"label":"Reset",
														"uid":"statMenu.reset"
													}
												]	
											},
											
											
											{
												"template":"spacerBar"
											},
											
											{
												"template":"fillLabelCenter",
												"label":"Curent XP",
												"value":0.75,
												"divisions":1000,
												"uid":"statMenu.xp"
											},
											{
												"template":"fillLabelCenter",
												"label":"Curent Level",
												"divisions":60,
												"value":0.25,
												"uid":"statMenu.level"
											}
											
											
											
											
										]
									},
									{
										"template":"cont",
										"fillRatioX":1,
										"fillDir":1,
										"children":[
											{
												"template":"sectionBar",
												"label":"Status"
											},
											
											
											{
												"template":"spacerBar"
											},
											
											{
												"template":"cont",
												"fillDir":1,
												"fillRatioX":1,
												"maxDimY": 512,
												"minDimY": 32,
												
												
												"dataFile":"E_SDT_STATUSDATA",
												"dataSource":"status",
												"childType":"E_GCT_AUTOMATIC",
												"children":[],
												
												"childTemplate":{
													"template":"fillLabelCenter",
													"label":"",
													"value":0,
													"divisions":10,
													"uid":"#statMenu.status"
												}
											},

											{
												"template":"spacerBar"
											}



											
											
										]
									}
																
								]
								
							}
						]
					}
					
					
					
					
				]
			},
			
			
			
			
			
			
			
			
			
			{
				"uid":"guiHandles.E_FM_MAINMENU",
				"template":"rootCont",
				"hasBackground":0,
				"children":[
					{
						"template":"cont",
						"fillDir":1,
						"children": [
						
							{
								"template":"menuBar",
								"label":"Main Menu"
								
							},
							{
								"template":"cont",
								"fillDir":1,
								"children":[
									
																		
									{
										"template":"radioFillCenter",
										"label":"Options",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
												
												
												{
													"template":"cont",
													"fillDir":1,
													"children": [
														{
															"template":"menuBar",
															"label":"Options"
															
														},
														{
															"template":"cont",
															"fillDir":1,
															"children":[
																{
																	"template":"radioFillCenter",
																	"label":"Graphics Options",
																	"floatingChildren": [{
																		"template":"popout",
																		"fillRatioX":1,
																		"children": [
																			
																			{
																				"template":"cont",
																				"fillDir":1,
																				"children": [
																				
																					{
																						"template":"menuBar",
																						"label":"Graphics Options"
																						
																					},
																					{
																						"template":"cont",
																						"fillDir":1,
																						"children":[
																							{
																								"template":"fillSlider",
																								"label":"Clip Distance",
																								"uid":"$options.graphics.clipDist"
																							},
																							{
																								"template":"fillSlider",
																								"label":"Field of View",
																								"uid":"$options.graphics.fov"
																							},
																							{
																								"template":"fillSlider",
																								"label":"SphereMap Precision",
																								"uid":"$options.graphics.sphereMapPrec"
																							},
																							{
																								"template":"fillSlider",
																								"label":"Max Height",
																								"uid":"$options.graphics.maxHeight"
																							},
																							{
																								"template":"fillSlider",
																								"label":"Gamma",
																								"uid":"$options.graphics.gammaVal"
																							}
																						]
																					}
																				]
																			}
																			
																		]
																	}]
																},
																{
																	"template":"radioFillCenter",
																	"label":"Sound Options",
																	"floatingChildren": [{
																		"template":"popout",
																		"fillRatioX":1,
																		"children": [
																			{
																				"template":"cont",
																				"fillDir":1,
																				"children": [
																				
																					{
																						"template":"menuBar",
																						"label":"Sound Options"
																						
																					},
																					{
																						"template":"cont",
																						"fillDir":1,
																						"children":[
																							{
																								"template":"fillSlider",
																								"label":"Master Volume",
																								"uid":"$options.sound.masterVolume"
																							},
																							{
																								"template":"fillSlider",
																								"label":"Ambient Volume",
																								"uid":"$options.sound.ambientVolume"
																							},
																							{
																								"template":"fillSlider",
																								"label":"GUI Volume",
																								"uid":"$options.sound.guiVolume"
																							},
																							{
																								"template":"fillSlider",
																								"label":"Music Volume",
																								"uid":"$options.sound.musicVolume"
																							},
																							{
																								"template":"fillSlider",
																								"label":"Sound FX Volume",
																								"uid":"$options.sound.fxVolume"
																							}
																						]
																					}
																				]
																			}
																		]
																	}]
																},
																{
																	"template":"radioFillCenter",
																	"label":"Input Options",
																	"floatingChildren": [{
																		"template":"popout",
																		"fillRatioX":1,
																		"children": [
																		
																		]
																	}]
																}
																
															]
														}
													]
												}
												
												
												
												
												
												
												
												
												
												
												
											]
										}]
									},
									
									
									{
										"template":"radioFillCenter",
										"label":"Materials",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"menuBar",
															"label":"Materials"
															
														},
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"maxDimY": 512,
															"uid":"materialEditor",
															"dataFile":"materials.js",
															"dataSource":"materials",
															"childType":"E_GCT_GENERIC",
															"children": [],
															"childTemplate":{
																"template":"radioFillCenter",
																"label":"",
																"floatingChildren":[
																
																
																
																
																
																	{
																		"template":"popout",
																		"fillRatioX":1,
																		"children": [
																			{
																				"template":"cont",
																				"fillDir":1,
																				"children": [
																					{
																						"template":"menuBar",
																						"label":"Material Nodes"
																						
																					},
																					{
																						"template":"fillContMat",
																						"label":" ",
																						"minDimY":140,
																						"flags":16
																					},
																					{
																						"template":"cont",
																						"fillDir":1,
																						"fillRatioX":1,
																						"dataFile":"materials.js",
																						"dataSource":"",
																						"childType":"E_GCT_GENERIC",
																						"children": [],
																						"childTemplate":{
																							"template":"cont",
																							"fillDir":0,
																							"fillRatioX":1,
																							"dataFile":"materials.js",
																							"dataSource":"",
																							"childType":"E_GCT_GENERIC",
																							"children": [],
																							"childTemplate":
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							{
																								"template":"radioFillCenterColor",
																								"label":"",
																								
																								"value0":0.0,
																								"value1":0.0,
																								"value2":0.0,
																								"value3":0.0,
																								
																								"dataFile":"",
																								"dataRef":"",
																								"dataKey":"i0_color",
																								
																								"uid":"#colorNode",
																								
																								
																								"floatingChildren":[{
																										"template":"popout",
																										"fillRatioX":1,
																										"children": [
																											{
																												"template":"cont",
																												"fillDir":1,
																												"children": [
																													{
																														"template":"menuBar",
																														"label":"Material Props"
																														
																													},
																													{
																														"template":"cont",
																														"fillDir":1,
																														"fillRatioX":1,
																														
																														"children": [
																																																														
																															{
																																"template":"fillSliderHSV",
																																"label":" ",
																																"minDimY":140,
																																"flags":5,
																																"valRef":"#colorNode",
																																"uid":"#materialVal"
																															},
																															{
																																"template":"fillSliderHSV",
																																"label":" ",
																																"minDimY":140,
																																"flags":6,
																																"valRef":"#colorNode",
																																"uid":"#materialVal"
																															},
																															
																															
																															{
																																"template":"fillSliderHSV",
																																"label":"Hue",
																																"flags":1,
																																"valRef":"#colorNode",
																																"uid":"#materialVal"
																															},
																															{
																																"template":"fillSliderHSV",
																																"label":"Saturation",
																																"flags":2,
																																"valRef":"#colorNode",
																																"uid":"#materialVal"
																															},
																															{
																																"template":"fillSliderHSV",
																																"label":"Lightness",
																																"flags":4,
																																"valRef":"#colorNode",
																																"uid":"#materialVal"
																																
																															},
																															
																															
																															{
																																"template":"fillSlider",
																																"label":"Power",
																																"dataKey":"i1_power",
																																"uid":"#materialVal"
																															},
																															{
																																"template":"fillSlider",
																																"label":"Ratio",
																																"dataKey":"i2_ratio",
																																"uid":"#materialVal"
																															}
																															
																															
																														]
																														
																													}
																												
																												]
																											}
																										]
																									}]
																							}
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																							
																						}
																						
																					}
																				
																				]
																			}
																		]
																	}
																
																
																
																
																
																
																]
															}
														}
													
													]
												}
											]
										}]
									},
									
									
									
									
									
									
									
									
									
									
									
									
									
									{
										"template":"radioFillCenter",
										"label":"Debug Info",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
											
												{
													"template":"cont",
													"fillDir":1,
													
													"children": [
													
														{
															"template":"menuBar",
															"label":"Debug Info"
															
														},
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"children":[
																{
																	"template":"fillLabel",
																	"uid":"debug.fbMem"
																},
																{
																	"template":"fillLabel",
																	"uid":"debug.vertMem"
																},
																{
																	"template":"fillLabel",
																	"uid":"debug.totMem"
																},
																{
																	"template":"fillLabel",
																	"uid":"debug.numVoxels"
																}
															]
														}
													]
												}								
												
											]
										}]
									},
									{
										"template":"radioFillCenter",
										"label":"Character Editor",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
											
												{
													"template":"cont",
													"fillDir":1,
													
													"children": [
													
														{
															"template":"menuBar",
															"label":"Character Editor"
															
														},
														{
															"template":"cont",
															"fillDir":1,
															"fillRatioX":1,
															"children":[
																{
																	"template":"fillToggle",
																	"label":"Test Pathfinding",
																	"uid":"$charEdit.pathfindingOn"
																},
																{
																	"template":"fillToggle",
																	"label":"Edit Pose",
																	"uid":"$charEdit.editPose"
																},
																{
																	"template":"fillToggle",
																	"label":"Render Human",
																	"uid":"$charEdit.orgOn"
																},
																{
																	"template":"fillToggle",
																	"label":"Mirror Changes",
																	"uid":"$charEdit.mirrorOn"
																},
																{
																	"template":"fillToggle",
																	"label":"Apply to Children",
																	"uid":"$charEdit.applyToChildren"
																},
																{
																	"template":"fillButton",
																	"label":"Save Pose",
																	"uid":"charEdit.savePose"
																},
																{
																	"template":"fillButton",
																	"label":"Load Pose",
																	"uid":"charEdit.loadPose"
																}
															]
														}
													]
												}								
												
											]
										}]
									},
									{
										"template":"radioFillCenter",
										"label":"Shader Parameters",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
											
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"menuBar",
															"label":"Shader Parameters"
															
														},
														{
															"template":"cont",
															"maxDimY": 512,
															"fillDir":1,
															"dataFile":"E_SDT_SHADERPARAMS",
															"dataSource":"params",
															"childType":"E_GCT_SHADER_PARAM",
															"children": [],
															"childTemplate":{
																"fillRatioX":1,
																"template":"slider",
																"label":"",
																"uid":"",
																
																"callback":"updateShaderParam",
																"callbackData":{
																	"shaderName":"",
																	"paramName":""
																},
																
																"floatingChildren":[
																	{
																		"template":"tooltipValue",
																		"children":[
																			{
																				"template":"cont",
																				"label":"",
																				"ss":"tooltipSS"
																			}
																		]
																	}
																]
															}
														}
													]
												}
											

											]
										}]
										
									},
									{
										"template":"checkFillCenter",
										"label":"Map",
										"floatingChildren": [{
											"template":"popout",
											"fillRatioX":1,
											"children": [
											
												{
													"template":"cont",
													"fillDir":1,
													"children": [
													
														{
															"template":"menuBar",
															"label":"Map"
															
														},
														{
															"template":"dragPad",
															"hasBackground":1,
															"uid":"map.mapHolder",
															"minDimX": 512,
															"minDimY": 512,
															"fillDir":1,
															"children": [
																
															]
														}
													]
												}
											
											
												
												
											]
										}]
										
									}
									
									
									
								]
							}
						]
					}
				]
			}
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		]
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}

