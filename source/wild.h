#include <nds.h>
#include <stdarg.h>

extern char filename[200];
extern char full_file_path[256];
extern char title_buffer[32];
extern char text_conversion_buffer[64];
extern int current_player;
extern int player_offset;
extern u8 savefile[256 * 1024];
extern const char *acww_folder_prefix;

extern int mainBGText, mainBG256, mainBGBehind, subBGText, subBG256, subBGBehind;
extern u16 *mainBGMapText;
extern u16 *mainBGMap256;
extern u16 *mainBGMapBehind;
extern u16 *subBGMapText;
extern u16 *subBGMap256;
extern u16 *subBGMapBehind;

void set_savefile_u16(int index, u16 value);
u16 get_savefile_u16(int index);
const char *player_name(int which);
const char *player_name_or_null(int which);
const char *town_name();
const char *text_from_save(int index, int length);
void fix_invalid_filename_chars(char *buffer);
const char *town_name_for_filename();

int confirm_choice(const char *prompt);
int confirm_choice_on_screen(u16 *screen, const char *prompt);
int popup_notice(const char *prompt);
int popup_noticef(const char *fmt, ...);
int popup_noticef_on_screen(u16 *map, const char *fmt, ...);
#define ALL_FILES 0
#define SAVE_FILES 1
#define PATTERN_FILES 2
int choose_file(int file_type);
int choose_file_on_screen(u16 *map, int file_type);
void clear_screen(u16 *screen);
void clear_screen_256(u16 *map);
void map_print(u16 *map, int x, int y, const char *text);
void map_printf(u16 *map, int x, int y, const char *fmt, ...);
void map_box(u16 *map, int x, int y, int w, int h);
void map_put(u16 *map, int x, int y, u16 c);
void map_rectfill(u16 *map, int x, int y, int w, int h, u16 c);
int choose_from_list(const char *prompt, const char **choices, int choice_count, int current_choice);
int choose_from_list_on_screen(u16 *map, const char *prompt, const char **choices, int choice_count, int current_choice);
int choose_item_from_all_on_screen(u16 *map, const char *prompt, u16 initial_item);
void wait_for_start();
void wait_vblank_and_animate();
void upload_pattern_palette();

extern const char *ok_options[];

const char *name_for_item(unsigned short item_id);
unsigned short icon_for_item(unsigned short item_id);

// Special tileset characters
enum {
	TILE_ARROW_ACTIVE   = 0xf0,
	TILE_ARROW_INACTIVE = 0xf1,
	TILE_BORDER_HORIZ   = 0xf2,
	TILE_BORDER_VERT    = 0xf3,
	TILE_BORDER_DR = 0xf4,
	TILE_BORDER_DL = 0xf5,
	TILE_BORDER_UR = 0xf6,
	TILE_BORDER_UL = 0xf7,
	TILE_BORDER_BOX = 0xf8,
	TILE_BUTTON_A = 0xe0,
	TILE_BUTTON_B = 0xe1,
	TILE_BUTTON_X = 0xe2,
	TILE_BUTTON_Y = 0xe3,
	TILE_BUTTON_L = 0xe4,
	TILE_BUTTON_R = 0xe5,
	TILE_VERTICAL_PICKER_L = 0xb0,
	TILE_VERTICAL_PICKER_R = 0xb1,
	TILE_VERTICAL_PICKER_L_INACTIVE = 0xb2,
	TILE_VERTICAL_PICKER_R_INACTIVE = 0xb3,
	TILE_VERTICAL_PICKER_L_BAR = 0xb4,
	TILE_VERTICAL_PICKER_R_BAR = 0xb5,
	TILE_VERTICAL_PICKER_L_BAR_INACTIVE = 0xb6,
	TILE_VERTICAL_PICKER_R_BAR_INACTIVE = 0xb7,
	TILE_SELECTION_BOX_TOP_LEFT = 0xe7,
	TILE_SELECTION_BOX_TOP      = 0xe8,
	TILE_SELECTION_BOX_LEFT     = 0xe9,
	TILE_BORDER_GRAY_CORNER = 0xf9,
	TILE_BORDER_GRAY_VERT   = 0xfa,
	TILE_BORDER_GRAY_HORIZ  = 0xfb,
};

#define SPRITE_MAIN_TILE_POINTER(num) (&SPRITE_GFX[16*num])
#define SPRITE_SUB_TILE_POINTER(num) (&SPRITE_GFX_SUB[16*num])

#define EXTRA_STORAGE_SIZE 1500
#define EXTRA_PATTERN_STORAGE_SIZE 1024

// Item code related
#define FURNITURE_FACING_DOWN  0
#define FURNITURE_FACING_RIGHT 1
#define FURNITURE_FACING_UP    2
#define FURNITURE_FACING_LEFT  3

#define AREA_NOT_AVAILABLE 0xf030
#define EMPTY_ITEM 0xfff1
#define INVALID_ITEM_SLOT 0xffff

// Savefile offsets
#define TOWN_SAVE_SIZE 0x15FE0
#define PER_PLAYER_OFFSET 8844
#define TOWN_ITEM_GRID_START 0x0C354
#define TOWN_ITEM_GRID_END   0x0E352
