#pragma once

namespace lra {
	enum Players {
		NONE = 0,
		COMPUTER,
		HUMAN,
	};

	struct TicTacTe {
		Players board[3][3];
	};
}
