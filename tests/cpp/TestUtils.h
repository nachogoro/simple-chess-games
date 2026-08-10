#ifndef TEST_UTILS_H_5B21F0A4_1C33_4C7E_9F84_2E0B6D0E9A11
#define TEST_UTILS_H_5B21F0A4_1C33_4C7E_9F84_2E0B6D0E9A11

#include <simplechess/SimpleChess.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <set>
#include <vector>

#define EXPECT_THROW_CUSTOM(OP, EXCEP) \
	do { \
		try \
		{ \
			OP; \
			FAIL() << "Expected EXCEP"; \
		} \
		catch (const EXCEP& e) \
		{ \
		} \
		catch (...) \
		{ \
			FAIL() << "Expected EXCEP"; \
		} \
	} while (0)

/**
 * \brief How many times \p move appears among \p moves.
 *
 * The tests assert on the presence or absence of individual moves, which is
 * what std::set::count used to express before the available moves became a
 * vector.
 */
inline size_t countMoves(
		const std::vector<simplechess::PieceMove>& moves,
		const simplechess::PieceMove& move)
{
	return static_cast<size_t>(
			std::count(moves.begin(), moves.end(), move));
}

/**
 * \brief The contents of \p moves in order.
 *
 * Whole-position expectations are written as sets, which makes a missing or
 * repeated move obvious to read. A set iterates in exactly the order \ref
 * simplechess::Game::allAvailableMoves() promises, so comparing its flattened
 * contents against the returned vector checks that ordering as well as the
 * membership.
 */
inline std::vector<simplechess::PieceMove> asVector(
		const std::set<simplechess::PieceMove>& moves)
{
	return {moves.begin(), moves.end()};
}

#endif
