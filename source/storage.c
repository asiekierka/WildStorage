/*
  WildStorage
  A tool for ACWW to add extra item storage

  Copyright (c) 2024 NovaSquirrel

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  -----

  This program references research and code from Animal Map

  (c) 2005-2007 DsPet
  Released as FreeWare and Open-source
  Additional modifications thanks to Virus of Haxxor World
*/
#include <nds.h>
#include "wild.h"
#include "acstr.h"

// Prototypes -----------------------------------
const char *text_from_save(int index, int length);

// Variables ------------------------------------
int storage_y = 0;
int storage_screen = 0;
int storage_screen_inventory[2];
int storage_page[2];
u16 *storage_map;

int storage_swap_screen;
int storage_swap_page;
int storage_swap_y;
bool storage_swapping = false;

u16 extra_storage_1[1500];
u16 extra_storage_2[1500];
u16 extra_storage_3[1500];

// For personalizing the options
char player_1_inventory_text[32];
char player_2_inventory_text[32];
char player_3_inventory_text[32];
char player_4_inventory_text[32];
char player_1_closet_text[32];
char player_2_closet_text[32];
char player_3_closet_text[32];
char player_4_closet_text[32];
char player_1_mail_text[32];
char player_2_mail_text[32];
char player_3_mail_text[32];
char player_4_mail_text[32];

const char *storage_inventory_names[] = {
	player_1_inventory_text, player_1_closet_text, player_1_mail_text,
	player_2_inventory_text, player_2_closet_text, player_2_mail_text,
	player_3_inventory_text, player_3_closet_text, player_3_mail_text,
	player_4_inventory_text, player_4_closet_text, player_4_mail_text,
	"Recycler bin",       "Lost and found",     "Shop",
	"Extra storage 1",    "Extra storage 2", "Extra storage 3",
};

const size_t inventory_size[] = {
	15, 90, 10,
	15, 90, 10,
	15, 90, 10,
	15, 90, 10,
	15, 15, 36,
	1500, 1500, 1500,
};

// Functions ------------------------------------

void set_savefile_u16(int index, u16 value) {
	*((u16*)&savefile[index])= value;
}

u16 get_savefile_u16(int index) {
	return *((u16*)&savefile[index]);
}

u16 get_inventory_item(int inventory, int slot) {
	if(slot > inventory_size[inventory])
		return 0xffff; // Unavailable slot
	switch(inventory) {
		case 0:	 return get_savefile_u16(0x01B2E + 8844*0 + slot*2); // Inventory
		case 1:  return get_savefile_u16(0x15430 + 180*0  + slot*2); // Closet
		case 2:  return 0;

		case 3:	 return get_savefile_u16(0x01B2E + 8844*1 + slot*2); // Inventory
		case 4:  return get_savefile_u16(0x15430 + 180*1  + slot*2); // Closet
		case 5:  return 0;

		case 6:	 return get_savefile_u16(0x01B2E + 8844*2 + slot*2); // Inventory
		case 7:  return get_savefile_u16(0x15430 + 180*2  + slot*2); // Closet
		case 8:  return 0;

		case 9:	 return get_savefile_u16(0x01B2E + 8844*3 + slot*2); // Inventory
		case 10: return get_savefile_u16(0x15430 + 180*3  + slot*2); // Closet
		case 11: return 0;

		case 12: return get_savefile_u16(0x15EDE + slot*2); // Recycler
		case 13: return get_savefile_u16(0x15EC0 + slot*2); // Lost and found
		case 14: return get_savefile_u16(0x15DB8 + slot*2); // Shop

		case 15: return extra_storage_1[slot];
		case 16: return extra_storage_2[slot];
		case 17: return extra_storage_3[slot];
	}
	return INVALID_ITEM_SLOT;
}

void set_inventory_item(int inventory, int slot, u16 item) {
	if(slot > inventory_size[inventory])
		return;
	switch(inventory) {
		case 0:	 set_savefile_u16(0x01B2E + 8844*0 + slot*2, item); break; // Inventory
		case 1:  set_savefile_u16(0x15430 + 180*0  + slot*2, item); break; // Closet
		case 2:  break;

		case 3:	 set_savefile_u16(0x01B2E + 8844*1 + slot*2, item); break; // Inventory
		case 4:  set_savefile_u16(0x15430 + 180*1  + slot*2, item); break; // Closet
		case 5:  break;

		case 6:	 set_savefile_u16(0x01B2E + 8844*2 + slot*2, item); break; // Inventory
		case 7:  set_savefile_u16(0x15430 + 180*2  + slot*2, item); break; // Closet
		case 8:  break;

		case 9:	 set_savefile_u16(0x01B2E + 8844*3 + slot*2, item); break; // Inventory
		case 10: set_savefile_u16(0x15430 + 180*3  + slot*2, item); break; // Closet
		case 11: break;

		case 12: set_savefile_u16(0x15EDE + slot*2, item); break; // Recycler
		case 13: set_savefile_u16(0x15EC0 + slot*2, item); break; // Lost and found
		case 14: set_savefile_u16(0x15DB8 + slot*2, item); break; // Shop

		case 15: extra_storage_1[slot] = item; break;
		case 16: extra_storage_2[slot] = item; break;
		case 17: extra_storage_3[slot] = item; break;
	}
}

int calc_page_count(int size, int page_size) {
	if(size % page_size)
		return size/page_size+1;
	return size/page_size;
}

void redraw_storage_screens() {
	clear_screen(mainBGMap);
	clear_screen(subBGMap);

	int page_count;

	// Upper screen
	page_count = calc_page_count(inventory_size[storage_screen_inventory[0]], 15);
	if(page_count > 1)
		map_printf(subBGMap, 1, 1, "%s (%d/%d)", storage_inventory_names[storage_screen_inventory[0]], storage_page[0]+1, page_count);
	else
		map_print(subBGMap, 1, 1, storage_inventory_names[storage_screen_inventory[0]]);
	map_box(subBGMap,   0, 2, 32, 17);

	// Bottom screen
	page_count = calc_page_count(inventory_size[storage_screen_inventory[1]], 15);
	if(page_count > 1)
		map_printf(mainBGMap, 1, 1, "%s (%d/%d)", storage_inventory_names[storage_screen_inventory[1]], storage_page[1]+1, page_count);
	else
		map_print(mainBGMap, 1, 1, storage_inventory_names[storage_screen_inventory[1]]);
	map_box(mainBGMap,   0, 2, 32, 17);

	map_print(mainBGMap, 1, 20, "\xe0:Move \xe1:Jump \xe2:Edit \xe3:Send");
	map_print(mainBGMap, 1, 21, "\xe4\xe5:Choose inventory");
	map_print(mainBGMap, 1, 22, "Start: Finish");

	// Cursors
	map_print(storage_map, 1, 3+storage_y, "\xf0");
	if(storage_swapping && storage_swap_page == storage_page[storage_swap_screen]) {
		map_print(storage_swap_screen ? mainBGMap : subBGMap, 1, 3+storage_swap_y, "\xf1");
	}

	// Display items
	for(int screen=0; screen<2; screen++) {
		u16 *screen_ptr = screen ? mainBGMap : subBGMap;
		for(int i=0; i<15; i++) {
			u16 item = get_inventory_item(storage_screen_inventory[screen], storage_page[screen]*15+i);
			if(item == INVALID_ITEM_SLOT) {
				map_print(screen_ptr, 2, 3+i, "\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0");
				continue;
			}
			if(item == EMPTY_ITEM)
				continue;
			map_put(screen_ptr,   2, 3+i, icon_for_item(item));
			map_print(screen_ptr, 3, 3+i, name_for_item(item));
		}
	}
}

void menu_storage() {
	// Set up inventory names from the player names
	const char *player_name;
	player_name = text_from_save(0x228E + 8844*0, 8);
	if(!player_name[0])
		player_name = "Player 1";
	sprintf(player_1_inventory_text, "\xC2%s's inventory", player_name);
	sprintf(player_1_closet_text, "\xD2%s's closet", player_name);
	sprintf(player_1_mail_text, "\xED%s's mail", player_name);

	player_name = text_from_save(0x228E + 8844*1, 8);
	if(!player_name[0])
		player_name = "Player 2";
	sprintf(player_2_inventory_text, "\xC2%s's inventory", player_name);
	sprintf(player_2_closet_text, "\xD2%s's closet", player_name);
	sprintf(player_2_mail_text, "\xED%s's mail", player_name);

	player_name = text_from_save(0x228E + 8844*2, 8);
	if(!player_name[0])
		player_name = "Player 3";
	sprintf(player_3_inventory_text, "\xC2%s's inventory", player_name);
	sprintf(player_3_closet_text, "\xD2%s's closet", player_name);
	sprintf(player_3_mail_text, "\xED%s's mail", player_name);

	player_name = text_from_save(0x228E + 8844*3, 8);
	if(!player_name[0])
		player_name = "Player 4";
	sprintf(player_4_inventory_text, "\xC2%s's inventory", player_name);
	sprintf(player_4_closet_text, "\xD2%s's closet", player_name);
	sprintf(player_4_mail_text, "\xED%s's mail", player_name);

	// ------------------------------------------

	storage_y = 0;
	storage_swapping = false;
	storage_screen = 0;
	storage_screen_inventory[0] = 0;
	storage_screen_inventory[1] = 0;
	storage_page[0] = 0;
	storage_page[1] = 0;
	storage_map = subBGMap;

	redraw_storage_screens();

	while(1) {
		swiWaitForVBlank();
		scanKeys();

		// Respond to button presses

		uint32_t keys_down = keysDown();
		uint32_t keys_repeat = 	keysDownRepeat();

		int old_y = storage_y;

		// Move between and within a page
		if(keys_repeat & KEY_UP) {
			storage_y--;
			if(storage_y < 0) {
				storage_y = 14;
				keys_down |= KEY_B;
			}
		}
		if(keys_repeat & KEY_DOWN) {
			storage_y++;
			if(storage_y >= 15) {
				storage_y = 0;
				keys_down |= KEY_B;
			}
		}
		if(keys_repeat & KEY_LEFT) {
			int size = inventory_size[storage_screen_inventory[storage_screen]];
			if(storage_page[storage_screen] > 0)
				storage_page[storage_screen]--;
			else if(size > 15) {
				// Go to the last page
				storage_page[storage_screen] = size/15 - (size%15 == 0);
			}
			redraw_storage_screens();
		}
		if(keys_repeat & KEY_RIGHT) {
			if((storage_page[storage_screen]+1)*15 < inventory_size[storage_screen_inventory[storage_screen]])
				storage_page[storage_screen]++;
			else
				storage_page[storage_screen] = 0;
			redraw_storage_screens();
		}

		// Choos new inventory
		if(keys_down & (KEY_L | KEY_R)) {
			int new_screen = choose_from_list_on_screen(storage_screen ? mainBGMap : subBGMap, "Choose box to display here", storage_inventory_names, 18, storage_screen_inventory[storage_screen]);
			if(new_screen >= 0) {
				storage_screen_inventory[storage_screen] = new_screen;
				storage_swapping = false;
				storage_y = 0;
				storage_page[storage_screen] = 0;
			}
			redraw_storage_screens();
		}

		// Redraw the cursor and erase it from the old spot
		if(storage_y != old_y) {
			map_print(storage_map, 1, 3+old_y,     " ");
			if(storage_swapping && storage_swap_page == storage_page[storage_swap_screen]) {
				map_print(storage_swap_screen ? mainBGMap : subBGMap, 1, 3+storage_swap_y, "\xf1");
			}
			map_print(storage_map, 1, 3+storage_y, "\xf0");
		}

		// Move/swap
		if(keys_down & KEY_A) {
			if(storage_swapping) {
				storage_swapping = false;
				// Erase old cursor
				if(storage_swap_screen != storage_screen || storage_y != storage_swap_y)
					map_print(storage_swap_screen ? mainBGMap : subBGMap, 1, 3+storage_swap_y, " ");

				// Actually attempt to swap the items
				int swap_index_1 = storage_page[storage_screen]*15+storage_y;
				int swap_index_2 = storage_page[storage_swap_screen]*15+storage_swap_y;
				u16 item1 = get_inventory_item(storage_screen_inventory[storage_screen], swap_index_1);
				u16 item2 = get_inventory_item(storage_screen_inventory[storage_swap_screen], swap_index_2);
				if(item1 != INVALID_ITEM_SLOT && item2 != INVALID_ITEM_SLOT) {
					set_inventory_item(storage_screen_inventory[storage_screen], swap_index_1, item2);
					set_inventory_item(storage_screen_inventory[storage_swap_screen], swap_index_2, item1);
					redraw_storage_screens();
				}
			} else {
				storage_swapping = true;
				storage_swap_screen = storage_screen;
				storage_swap_page = storage_page[storage_screen];
				storage_swap_y = storage_y;
			}
		}

		// Jump between screens
		if(keys_down & KEY_B) {
			storage_screen ^= 1;
			storage_map = (storage_screen) ? mainBGMap : subBGMap;
			redraw_storage_screens();
		}

		// Edit
		if(keys_down & KEY_X) {

		}

		// Send items over
		if(keys_down & KEY_Y) {
			storage_swapping = false;

			int starting_index = storage_page[storage_screen^1] * 15;
			for(int index_to_swap_to = starting_index; index_to_swap_to < starting_index + 15; index_to_swap_to++) {
				if(get_inventory_item(storage_screen_inventory[storage_screen^1], index_to_swap_to) == EMPTY_ITEM) {
					int index_to_swap_from = storage_page[storage_screen]*15+storage_y;
					u16 item_to_swap_in = get_inventory_item(storage_screen_inventory[storage_screen], index_to_swap_from);
					if(item_to_swap_in == EMPTY_ITEM)
						break;
					set_inventory_item(storage_screen_inventory[storage_screen^1], index_to_swap_to, item_to_swap_in);
					set_inventory_item(storage_screen_inventory[storage_screen], index_to_swap_from, EMPTY_ITEM);
					redraw_storage_screens();
					break;
				}
			}
		}

		// Exit
		if(keys_down & KEY_START)
			return;
	}
}