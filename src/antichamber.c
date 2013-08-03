#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define DATE 1

#define MY_UUID { 0x17, 0x90, 0xF9, 0xFE, 0x61, 0x71, 0x4C, 0xE7, 0x8F, 0xCB, 0xC8, 0x30, 0x32, 0x6A, 0xCF, 0xA4 }
PBL_APP_INFO(MY_UUID,
			"Antichamber", "Computing Eureka",
			1, 0, /* App version */
			DEFAULT_MENU_ICON,
			APP_INFO_WATCH_FACE);

Window window;
HeapBitmap bs[12];
HeapBitmap* bcolon;
HeapBitmap* bdash;
BitmapLayer bl[16];
int bsh[12] = {RESOURCE_ID_IMAGE_0, RESOURCE_ID_IMAGE_1, RESOURCE_ID_IMAGE_2, RESOURCE_ID_IMAGE_3, RESOURCE_ID_IMAGE_4, RESOURCE_ID_IMAGE_5, RESOURCE_ID_IMAGE_6, RESOURCE_ID_IMAGE_7, RESOURCE_ID_IMAGE_8, RESOURCE_ID_IMAGE_9, RESOURCE_ID_IMAGE_colon, RESOURCE_ID_IMAGE_dash};

void setSegs(BitmapLayer* ptr, int val)
{
	bitmap_layer_set_bitmap(&ptr[0], &bs[val/10].bmp);
	bitmap_layer_set_bitmap(&ptr[1], &bs[val%10].bmp);
}

void handle_second_tick(AppContextRef ctx, PebbleTickEvent* t)
{
	PblTm time;
	get_time(&time);

	if(!clock_is_24h_style() && time.tm_hour > 12)
	{
		time.tm_hour -= 12;
	}
	if(!clock_is_24h_style() && time.tm_hour == 0)
	{
		time.tm_hour = 12;
	}

	setSegs(&bl[0], time.tm_hour);
	setSegs(&bl[3], time.tm_min);
	setSegs(&bl[6], time.tm_sec);
	setSegs(&bl[8], time.tm_mon+1);
	setSegs(&bl[11], time.tm_mday+1);
	setSegs(&bl[14], (time.tm_year+1900)%100);
}

void handle_init(AppContextRef ctx)
{
	int i;

	resource_init_current_app(&RES_FOR_ANTICHAMBER);

	window_init(&window, "Antichamber");

	for(i = 0; i < 12; i++)
	{
		heap_bitmap_init(&bs[i], bsh[i]);
	}
	bcolon = &bs[10];
	bdash = &bs[11];

	for(i = 0; i < 8; i++)
		bitmap_layer_init(&bl[i], GRect(18*i,(168/2)-35,18,30));
	for(i = 8; i < 16; i++)
		bitmap_layer_init(&bl[i], GRect(18*(i-8),(168/2)+5,18,30));
	for(i = 0; i < 16; i++)
		layer_add_child(&window.layer, &bl[i].layer);

	window_set_background_color(&window, GColorBlack);

	bitmap_layer_set_bitmap(&bl[2], &bcolon->bmp);
	bitmap_layer_set_bitmap(&bl[5], &bcolon->bmp);
	bitmap_layer_set_bitmap(&bl[10], &bdash->bmp);
	bitmap_layer_set_bitmap(&bl[13], &bdash->bmp);

	handle_second_tick(ctx, NULL);

	window_stack_push(&window, true);
}

void handle_deinit(AppContextRef ctx)
{
	int i;

	for (i = 0; i < 12; i++)
	{
		heap_bitmap_deinit(&bs[i]);
	}
}

void pbl_main(void *params)
{
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,
		.deinit_handler = &handle_deinit,
		.tick_info = {
			.tick_handler = &handle_second_tick,
			.tick_units = SECOND_UNIT
		}
	};
	app_event_loop(params, &handlers);
}
