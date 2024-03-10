const Collision tilt_box_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(32),
	COL_VERTEX(-825, -1084, 230),
	COL_VERTEX(-825, -1039, 230),
	COL_VERTEX(-825, -1039, -230),
	COL_VERTEX(-825, -1084, -230),
	COL_VERTEX(825, -1039, -230),
	COL_VERTEX(825, -1084, -230),
	COL_VERTEX(825, -1039, 230),
	COL_VERTEX(825, -1084, 230),
	COL_VERTEX(-825, 1084, 230),
	COL_VERTEX(-825, 1084, -230),
	COL_VERTEX(-825, 1039, -230),
	COL_VERTEX(-825, 1039, 230),
	COL_VERTEX(825, 1084, -230),
	COL_VERTEX(825, 1039, -230),
	COL_VERTEX(825, 1084, 230),
	COL_VERTEX(825, 1039, 230),
	COL_VERTEX(1084, -825, 230),
	COL_VERTEX(1039, -825, 230),
	COL_VERTEX(1039, -825, -230),
	COL_VERTEX(1084, -825, -230),
	COL_VERTEX(1039, 825, -230),
	COL_VERTEX(1084, 825, -230),
	COL_VERTEX(1039, 825, 230),
	COL_VERTEX(1084, 825, 230),
	COL_VERTEX(-1084, -825, 230),
	COL_VERTEX(-1084, -825, -230),
	COL_VERTEX(-1039, -825, -230),
	COL_VERTEX(-1039, -825, 230),
	COL_VERTEX(-1084, 825, -230),
	COL_VERTEX(-1039, 825, -230),
	COL_VERTEX(-1084, 825, 230),
	COL_VERTEX(-1039, 825, 230),
	COL_TRI_INIT(SURFACE_SLIPPERY, 48),
	COL_TRI(0, 1, 2),
	COL_TRI(0, 2, 3),
	COL_TRI(3, 2, 4),
	COL_TRI(3, 4, 5),
	COL_TRI(5, 4, 6),
	COL_TRI(5, 6, 7),
	COL_TRI(7, 6, 1),
	COL_TRI(7, 1, 0),
	COL_TRI(3, 5, 7),
	COL_TRI(3, 7, 0),
	COL_TRI(4, 2, 1),
	COL_TRI(4, 1, 6),
	COL_TRI(8, 9, 10),
	COL_TRI(8, 10, 11),
	COL_TRI(9, 12, 13),
	COL_TRI(9, 13, 10),
	COL_TRI(12, 14, 15),
	COL_TRI(12, 15, 13),
	COL_TRI(14, 8, 11),
	COL_TRI(14, 11, 15),
	COL_TRI(9, 8, 14),
	COL_TRI(9, 14, 12),
	COL_TRI(13, 15, 11),
	COL_TRI(13, 11, 10),
	COL_TRI(16, 17, 18),
	COL_TRI(16, 18, 19),
	COL_TRI(19, 18, 20),
	COL_TRI(19, 20, 21),
	COL_TRI(21, 20, 22),
	COL_TRI(21, 22, 23),
	COL_TRI(23, 22, 17),
	COL_TRI(23, 17, 16),
	COL_TRI(19, 21, 23),
	COL_TRI(19, 23, 16),
	COL_TRI(20, 18, 17),
	COL_TRI(20, 17, 22),
	COL_TRI(24, 25, 26),
	COL_TRI(24, 26, 27),
	COL_TRI(25, 28, 29),
	COL_TRI(25, 29, 26),
	COL_TRI(28, 30, 31),
	COL_TRI(28, 31, 29),
	COL_TRI(30, 24, 27),
	COL_TRI(30, 27, 31),
	COL_TRI(25, 24, 30),
	COL_TRI(25, 30, 28),
	COL_TRI(29, 31, 27),
	COL_TRI(29, 27, 26),
	COL_TRI_STOP(),
	COL_END()
};
