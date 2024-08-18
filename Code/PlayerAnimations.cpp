#include "Objects/Player.h"

std::vector<std::vector<Player::Animation>> Player::PlayerAnimations =
{
	// Small robot
	{
		// 0 -> Idle
		Player::Animation
		{
			.Scale = 0.25f,
			.Speed = 1,
			.MeshFiles = {
				"Small_Idle0"
			},
		},
		// 1 -> Walk
		Player::Animation
		{
			.Scale = 0.25f,
			.Speed = 0.1f,
			.MeshFiles = {
				"Small_Walk0",
				"Small_Walk1",
				"Small_Walk2",
				"Small_Walk3",
			},
		},
		// 2 -> WallSlide
		Player::Animation
		{
			.Scale = 0.25f,
			.Speed = 0.2f,
			.MeshFiles = {
				"Small_WallSlide0",
				"Small_WallSlide1",
			},
		},
		// 3 -> Jump
		Player::Animation
		{
			.Scale = 0.25f,
			.Speed = 0.2f,
			.MeshFiles = {
				"Small_Jump0",
			},
		},
		// 4 -> Falling
		Player::Animation
		{
			.Scale = 0.25f,
			.Speed = 0.2f,
			.MeshFiles = {
				"Small_Falling0",
				"Small_Falling1",
			},
		},
		// 5 -> Gliding
		Player::Animation
		{
			.Scale = 0.25f,
			.Speed = 0.15f,
			.MeshFiles = {
				"Small_Gliding0",
				"Small_Gliding1",
			},
		},
	},
	// Medium robot
	{
		// 0 -> Idle
		Player::Animation
		{
			.Scale = 0.5f,
			.Speed = 1,
			.MeshFiles = {
				"Medium_Idle0"
			},
		},
		// 1 -> Walk
		Player::Animation
		{
			.Scale = 0.5f,
			.Speed = 0.1f,
			.MeshFiles = {
				"Medium_Walk0",
				"Medium_Walk1",
				"Medium_Walk2",
			},
		},
		// 2 -> WallSlide
		Player::Animation
		{
			.Scale = 0.5f,
			.Speed = 0.2f,
			.MeshFiles = {
				"Medium_WallSlide0"
			},
		},
		// 3 -> Jump
		Player::Animation
		{
			.Scale = 0.5f,
			.Speed = 0.2f,
			.MeshFiles = {
				"Medium_Jump0",
			},
		},
		// 4 -> Falling
		Player::Animation
		{
			.Scale = 0.5f,
			.Speed = 0.1f,
			.MeshFiles = {
				"Medium_Fall0",
				"Medium_Fall1",
			},
		},
	},
	// Large robot
	{
		// 0 -> Idle
		Player::Animation
		{
			.Scale = 1.35f,
			.Speed = 1,
			.MeshFiles = {
				"Large_Idle0"
			},
		},
		// 1 -> Walk
		Player::Animation
		{
			.Scale = 1.35f,
			.Speed = 0.1f,
			.MeshFiles = {
				"Large_Walk0",
				"Large_Walk1",
				"Large_Walk2",
			},
		},
		// 2 -> WallSlide (Unused)
		Player::Animation
		{
			.Scale = 1.35f,
			.Speed = 0.2f,
			.MeshFiles = {
				"Large_Idle0"
			},
		},
		// 3 -> Jump
		Player::Animation
		{
			.Scale = 1.35f,
			.Speed = 0.2f,
			.MeshFiles = {
				"Large_Jump0",
			},
		},
		// 4 -> Falling
		Player::Animation
		{
			.Scale = 1.35f,
			.Speed = 0.1f,
			.MeshFiles = {
				"Large_Falling0",
				"Large_Falling1",
			},
		},
	}
};