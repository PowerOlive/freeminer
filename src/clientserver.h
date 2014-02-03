/*
Minetest
Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef CLIENTSERVER_HEADER
#define CLIENTSERVER_HEADER

#include <vector>
#include <utility>
#include <string>
#include "irrlichttypes.h"
#include <msgpack.hpp>

/*
	changes by PROTOCOL_VERSION:

	PROTOCOL_VERSION 3:
		Base for writing changes here
	PROTOCOL_VERSION 4:
		Add TOCLIENT_MEDIA
		Add TOCLIENT_TOOLDEF
		Add TOCLIENT_NODEDEF
		Add TOCLIENT_CRAFTITEMDEF
		Add TOSERVER_INTERACT
		Obsolete TOSERVER_CLICK_ACTIVEOBJECT
		Obsolete TOSERVER_GROUND_ACTION
	PROTOCOL_VERSION 5:
		Make players to be handled mostly as ActiveObjects
	PROTOCOL_VERSION 6:
		Only non-cached textures are sent
	PROTOCOL_VERSION 7:
		Add TOCLIENT_ITEMDEF
		Obsolete TOCLIENT_TOOLDEF
		Obsolete TOCLIENT_CRAFTITEMDEF
		Compress the contents of TOCLIENT_ITEMDEF and TOCLIENT_NODEDEF
	PROTOCOL_VERSION 8:
		Digging based on item groups
		Many things
	PROTOCOL_VERSION 9:
		ContentFeatures and NodeDefManager use a different serialization
		    format; better for future version cross-compatibility
		Many things
	PROTOCOL_VERSION 10:
		TOCLIENT_PRIVILEGES
		Version raised to force 'fly' and 'fast' privileges into effect.
		Node metadata change (came in later; somewhat incompatible)
	PROTOCOL_VERSION 11:
		TileDef in ContentFeatures
		Nodebox drawtype
		(some dev snapshot)
		TOCLIENT_INVENTORY_FORMSPEC
		(0.4.0, 0.4.1)
	PROTOCOL_VERSION 12:
		TOSERVER_INVENTORY_FIELDS
		16-bit node ids
		TOCLIENT_DETACHED_INVENTORY
	PROTOCOL_VERSION 13:
		InventoryList field "Width" (deserialization fails with old versions)
	PROTOCOL_VERSION 14:
		Added transfer of player pressed keys to the server
		Added new messages for mesh and bone animation, as well as attachments
		GENERIC_CMD_SET_ANIMATION
		GENERIC_CMD_SET_BONE_POSITION
		GENERIC_CMD_SET_ATTACHMENT
	PROTOCOL_VERSION 15:
		Serialization format changes
	PROTOCOL_VERSION 16:
		TOCLIENT_SHOW_FORMSPEC
	PROTOCOL_VERSION 17:
		Serialization format change: include backface_culling flag in TileDef
		Added rightclickable field in nodedef
		TOCLIENT_SPAWN_PARTICLE
		TOCLIENT_ADD_PARTICLESPAWNER
		TOCLIENT_DELETE_PARTICLESPAWNER
	PROTOCOL_VERSION 18:
		damageGroups added to ToolCapabilities
		sound_place added to ItemDefinition
	PROTOCOL_VERSION 19:
		GENERIC_CMD_SET_PHYSICS_OVERRIDE
	PROTOCOL_VERSION 20:
		TOCLIENT_HUDADD
		TOCLIENT_HUDRM
		TOCLIENT_HUDCHANGE
		TOCLIENT_HUD_SET_FLAGS
	PROTOCOL_VERSION 21:
		TOCLIENT_BREATH
		TOSERVER_BREATH
		range added to ItemDefinition
		drowning, leveled added to ContentFeatures
		stepheight and collideWithObjects added to object properties
		version, heat and humidity transfer in MapBock
		automatic_face_movement_dir and automatic_face_movement_dir_offset
			added to object properties
	PROTOCOL_VERSION 23:
		TOCLIENT_ANIMATIONS
*/

#define LATEST_PROTOCOL_VERSION 23

// Server's supported network protocol range
#define SERVER_PROTOCOL_VERSION_MIN 13
#define SERVER_PROTOCOL_VERSION_MAX LATEST_PROTOCOL_VERSION

// Client's supported network protocol range
#define CLIENT_PROTOCOL_VERSION_MIN 13
#define CLIENT_PROTOCOL_VERSION_MAX LATEST_PROTOCOL_VERSION

// Constant that differentiates the protocol from random data and other protocols
#define PROTOCOL_ID 0x4f457403

#define PASSWORD_SIZE 28       // Maximum password length. Allows for
                               // base64-encoded SHA-1 (27+\0).

#define TEXTURENAME_ALLOWED_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_."

#define TOCLIENT_INIT 0x10
enum {
	// u8 deployed version
	TOCLIENT_INIT_DEPLOYED,
	// u64 map seed
	TOCLIENT_INIT_SEED,
	// float recommended send interval (server step)
	TOCLIENT_INIT_STEP,
	// v3f player's position
	TOCLIENT_INIT_POS
};

#define TOCLIENT_REMOVENODE 0x22
enum {
	TOCLIENT_REMOVENODE_POS
};

#define TOCLIENT_BREATH 0x4e
enum {
	// u16 breath
	TOCLIENT_BREATH_BREATH
};

#define TOCLIENT_TIME_OF_DAY 0x29
enum {
	// u16 time (0-23999)
	TOCLIENT_TIME_OF_DAY_TIME,
	// f32 time_speed
	TOCLIENT_TIME_OF_DAY_TIME_SPEED
};

#define TOCLIENT_ACTIVE_OBJECT_REMOVE_ADD 0x31
enum {
	// list of ids
	TOCLIENT_ACTIVE_OBJECT_REMOVE_ADD_REMOVE,
	// list of [id, type, initialization_data]
	TOCLIENT_ACTIVE_OBJECT_REMOVE_ADD_ADD
};

struct ActiveObjectAddData {
	ActiveObjectAddData(u16 id_, u8 type_, std::string data_) : id(id_), type(type_), data(data_) {}
	ActiveObjectAddData() : id(0), type(0), data("") {}
	u16 id;
	u8 type;
	std::string data;
	MSGPACK_DEFINE(id, type, data)
};

#define TOCLIENT_ACTIVE_OBJECT_MESSAGES 0x32
enum {
	// list of pair<id, message> where id is u16 and message is string
	TOCLIENT_ACTIVE_OBJECT_MESSAGES_MESSAGES
};
typedef std::vector<std::pair<unsigned int, std::string> > ActiveObjectMessages;

#define TOCLIENT_HP 0x33
enum {
	TOCLIENT_HP_HP
};

#define TOCLIENT_MOVE_PLAYER 0x34
enum {
	// v3f player position
	TOCLIENT_MOVE_PLAYER_POS,
	// f32 pitch
	TOCLIENT_MOVE_PLAYER_PITCH,
	// f32 yaw
	TOCLIENT_MOVE_PLAYER_YAW
};

#define TOCLIENT_DEATHSCREEN 0x37
enum {
	// bool set camera point target
	TOCLIENT_DEATHSCREEN_SET_CAMERA,
	// v3f camera point target (to point the death cause or whatever)
	TOCLIENT_DEATHSCREEN_CAMERA_POINT
};

#define TOCLIENT_MEDIA 0x38
enum {
	// vector<pair<name, data>>
	TOCLIENT_MEDIA_MEDIA
};
typedef std::vector<std::pair<std::string, std::string> > MediaData;

#define TOCLIENT_ANNOUNCE_MEDIA 0x3c
enum {
	// list of [string name, string sha1_digest]
	TOCLIENT_ANNOUNCE_MEDIA_LIST,
	// string, url of remote media server
	TOCLIENT_ANNOUNCE_MEDIA_REMOTE_SERVER
};
typedef std::vector<std::pair<std::string, std::string> > MediaAnnounceList;

#define TOCLIENT_PLAY_SOUND 0x3f
enum {
	// s32
	TOCLIENT_PLAY_SOUND_ID,
	// string
	TOCLIENT_PLAY_SOUND_NAME,
	// f32
	TOCLIENT_PLAY_SOUND_GAIN,
	// u8
	TOCLIENT_PLAY_SOUND_TYPE,
	// v3f
	TOCLIENT_PLAY_SOUND_POS,
	// u16
	TOCLIENT_PLAY_SOUND_OBJECT_ID,
	// bool
	TOCLIENT_PLAY_SOUND_LOOP
};

#define TOCLIENT_PRIVILEGES 0x41
enum {
	// list of strings
	TOCLIENT_PRIVILEGES_PRIVILEGES
};

#define TOCLIENT_STOP_SOUND 0x40
enum {
	// s32
	TOCLIENT_STOP_SOUND_ID
};

#define TOCLIENT_INVENTORY_FORMSPEC 0x42
enum {
	// string
	TOCLIENT_INVENTORY_FORMSPEC_DATA
};

#define TOCLIENT_SHOW_FORMSPEC 0x44
enum {
	// string formspec
	TOCLIENT_SHOW_FORMSPEC_DATA,
	// string formname
	TOCLIENT_SHOW_FORMSPEC_NAME
};

#define TOCLIENT_CHAT_MESSAGE 0x30
enum {
	// string
	TOCLIENT_CHAT_MESSAGE_DATA
};

#define TOCLIENT_ACCESS_DENIED 0x35
enum {
	// string
	TOCLIENT_ACCESS_DENIED_REASON
};

#define TOCLIENT_NODEDEF 0x3a
enum {
	TOCLIENT_NODEDEF_DEFINITIONS
};

#define TOCLIENT_ITEMDEF 0x3d
enum {
	TOCLIENT_ITEMDEF_DEFINITIONS
};

#define TOCLIENT_INVENTORY 0x27
enum {
	// string, serialized inventory
	TOCLIENT_INVENTORY_DATA
};

#define TOCLIENT_DETACHED_INVENTORY 0x43
enum {
	TOCLIENT_DETACHED_INVENTORY_NAME,
	TOCLIENT_DETACHED_INVENTORY_DATA
};

#define TOCLIENT_MOVEMENT 0x45
// all values are floats here
enum {
	TOCLIENT_MOVEMENT_ACCELERATION_DEFAULT,
	TOCLIENT_MOVEMENT_ACCELERATION_AIR,
	TOCLIENT_MOVEMENT_ACCELERATION_FAST,
	TOCLIENT_MOVEMENT_SPEED_WALK,
	TOCLIENT_MOVEMENT_SPEED_CROUCH,
	TOCLIENT_MOVEMENT_SPEED_FAST,
	TOCLIENT_MOVEMENT_SPEED_CLIMB,
	TOCLIENT_MOVEMENT_SPEED_JUMP,
	TOCLIENT_MOVEMENT_LIQUID_FLUIDITY,
	TOCLIENT_MOVEMENT_LIQUID_FLUIDITY_SMOOTH,
	TOCLIENT_MOVEMENT_LIQUID_SINK,
	TOCLIENT_MOVEMENT_GRAVITY
};

enum ToClientCommand
{
	TOCLIENT_BLOCKDATA = 0x20, //TODO: Multiple blocks
	TOCLIENT_ADDNODE = 0x21,
	/*
		u16 command
		v3s16 position
		serialized mapnode
		u8 keep_metadata // Added in protocol version 22
	*/



	TOCLIENT_SPAWN_PARTICLE = 0x46,
	/*
		u16 command
		v3f1000 pos
		v3f1000 velocity
		v3f1000 acceleration
		f1000 expirationtime
		f1000 size
		u8 bool collisiondetection
		u8 bool vertical
		u32 len
		u8[len] texture
	*/

	TOCLIENT_ADD_PARTICLESPAWNER = 0x47,
	/*
		u16 command
		u16 amount
		f1000 spawntime
		v3f1000 minpos
		v3f1000 maxpos
		v3f1000 minvel
		v3f1000 maxvel
		v3f1000 minacc
		v3f1000 maxacc
		f1000 minexptime
		f1000 maxexptime
		f1000 minsize
		f1000 maxsize
		u8 bool collisiondetection
		u8 bool vertical
		u32 len
		u8[len] texture
		u32 id
	*/

	TOCLIENT_DELETE_PARTICLESPAWNER = 0x48,
	/*
		u16 command
		u32 id
	*/

	TOCLIENT_HUDADD = 0x49,
	/*
		u16 command
		u32 id
		u8 type
		v2f1000 pos
		u32 len
		u8[len] name
		v2f1000 scale
		u32 len2
		u8[len2] text
		u32 number
		u32 item
		u32 dir
		v2f1000 align
		v2f1000 offset
	*/

	TOCLIENT_HUDRM = 0x4a,
	/*
		u16 command
		u32 id
	*/

	TOCLIENT_HUDCHANGE = 0x4b,
	/*
		u16 command
		u32 id
		u8 stat
		[v2f1000 data |
		 u32 len
		 u8[len] data |
		 u32 data]
	*/

	TOCLIENT_HUD_SET_FLAGS = 0x4c,
	/*
		u16 command
		u32 flags
		u32 mask
	*/

	TOCLIENT_HUD_SET_PARAM = 0x4d,
	/*
		u16 command
		u16 param
		u16 len
		u8[len] value
	*/

	TOCLIENT_ANIMATIONS = 0x4f,
	/*
		u16 command
		f1000 animation_default_start
		f1000 animation_default_stop
		f1000 animation_walk_start
		f1000 animation_walk_stop
		f1000 animation_dig_start
		f1000 animation_dig_stop
		f1000 animation_wd_start
		f1000 animation_wd_stop
	*/
};

#define TOSERVER_INIT 0x10
enum {
	// u8 SER_FMT_VER_HIGHEST_READ
	TOSERVER_INIT_FMT,
	TOSERVER_INIT_NAME,
	TOSERVER_INIT_PASSWORD,
	TOSERVER_INIT_PROTOCOL_VERSION_MIN,
	TOSERVER_INIT_PROTOCOL_VERSION_MAX
};

#define TOSERVER_INIT2 0x11

#define TOSERVER_PLAYERPOS 0x23
enum {
	// v3f
	TOSERVER_PLAYERPOS_POSITION,
	// v3f
	TOSERVER_PLAYERPOS_SPEED,
	// f32
	TOSERVER_PLAYERPOS_PITCH,
	// f32
	TOSERVER_PLAYERPOS_YAW,
	// u32
	TOSERVER_PLAYERPOS_KEY_PRESSED
};

#define TOSERVER_CHAT_MESSAGE 0x32
enum {
	TOSERVER_CHAT_MESSAGE_DATA
};

enum ToServerCommand
{
	/*
		Sent first after connected.

		[0] u16 TOSERVER_INIT
		[3] u8[20] player_name
		[23] u8[28] password (new in some version)
		[51] u16 minimum supported network protocol version (added sometime)
		[53] u16 maximum supported network protocol version (added later than the previous one)
	*/

	// TOSERVER_INIT2 = 0x11,
	/*
		Sent as an ACK for TOCLIENT_INIT.
		After this, the server can send data.

		[0] u16 TOSERVER_INIT2
	*/

	TOSERVER_GETBLOCK=0x20, // Obsolete
	TOSERVER_ADDNODE = 0x21, // Obsolete
	TOSERVER_REMOVENODE = 0x22, // Obsolete


	TOSERVER_GOTBLOCKS = 0x24,
	/*
		[0] u16 command
		[2] u8 count
		[3] v3s16 pos_0
		[3+6] v3s16 pos_1
		[9] u16 wanted range
		...
	*/

	TOSERVER_DELETEDBLOCKS = 0x25,
	/*
		[0] u16 command
		[2] u8 count
		[3] v3s16 pos_0
		[3+6] v3s16 pos_1
		...
	*/

	TOSERVER_ADDNODE_FROM_INVENTORY = 0x26, // Obsolete
	/*
		[0] u16 command
		[2] v3s16 pos
		[8] u16 i
	*/

	TOSERVER_CLICK_OBJECT = 0x27, // Obsolete
	/*
		length: 13
		[0] u16 command
		[2] u8 button (0=left, 1=right)
		[3] v3s16 blockpos
		[9] s16 id
		[11] u16 item
	*/

	TOSERVER_GROUND_ACTION = 0x28, // Obsolete
	/*
		length: 17
		[0] u16 command
		[2] u8 action
		[3] v3s16 nodepos_undersurface
		[9] v3s16 nodepos_abovesurface
		[15] u16 item
		actions:
		0: start digging (from undersurface)
		1: place block (to abovesurface)
		2: stop digging (all parameters ignored)
		3: digging completed
	*/
	
	TOSERVER_RELEASE = 0x29, // Obsolete

	// (oops, there is some gap here)

	TOSERVER_SIGNTEXT = 0x30, // Old signs, obsolete
	/*
		u16 command
		v3s16 blockpos
		s16 id
		u16 textlen
		textdata
	*/

	TOSERVER_INVENTORY_ACTION = 0x31,
	/*
		See InventoryAction in inventory.h
	*/

	TOSERVER_SIGNNODETEXT = 0x33, // obsolete
	/*
		u16 command
		v3s16 p
		u16 textlen
		textdata
	*/

	TOSERVER_CLICK_ACTIVEOBJECT = 0x34, // Obsolete
	/*
		length: 7
		[0] u16 command
		[2] u8 button (0=left, 1=right)
		[3] u16 id
		[5] u16 item
	*/
	
	TOSERVER_DAMAGE = 0x35,
	/*
		u16 command
		u8 amount
	*/

	TOSERVER_PASSWORD=0x36,
	/*
		Sent to change password.

		[0] u16 TOSERVER_PASSWORD
		[2] u8[28] old password
		[30] u8[28] new password
	*/

	TOSERVER_PLAYERITEM=0x37,
	/*
		Sent to change selected item.

		[0] u16 TOSERVER_PLAYERITEM
		[2] u16 item
	*/
	
	TOSERVER_RESPAWN=0x38,
	/*
		u16 TOSERVER_RESPAWN
	*/

	TOSERVER_INTERACT = 0x39,
	/*
		[0] u16 command
		[2] u8 action
		[3] u16 item
		[5] u32 length of the next item
		[9] serialized PointedThing
		actions:
		0: start digging (from undersurface) or use
		1: stop digging (all parameters ignored)
		2: digging completed
		3: place block or item (to abovesurface)
		4: use item

		(Obsoletes TOSERVER_GROUND_ACTION and TOSERVER_CLICK_ACTIVEOBJECT.)
	*/
	
	TOSERVER_REMOVED_SOUNDS = 0x3a,
	/*
		u16 command
		u16 len
		s32[len] sound_id
	*/

	TOSERVER_NODEMETA_FIELDS = 0x3b,
	/*
		u16 command
		v3s16 p
		u16 len
		u8[len] form name (reserved for future use)
		u16 number of fields
		for each field:
			u16 len
			u8[len] field name
			u32 len
			u8[len] field value
	*/

	TOSERVER_INVENTORY_FIELDS = 0x3c,
	/*
		u16 command
		u16 len
		u8[len] form name (reserved for future use)
		u16 number of fields
		for each field:
			u16 len
			u8[len] field name
			u32 len
			u8[len] field value
	*/

	TOSERVER_REQUEST_MEDIA = 0x40,
	/*
		u16 command
		u16 number of files requested
		for each file {
			u16 length of name
			string name
		}
	 */

	TOSERVER_RECEIVED_MEDIA = 0x41,
	/*
		u16 command
	*/

	TOSERVER_BREATH = 0x42,
	/*
		u16 command
		u16 breath
	*/
};

#endif

