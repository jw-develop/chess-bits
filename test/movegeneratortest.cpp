/*
 * Copyright (C) 2013-2016 Phokham Nonava
 *
 * Use of this source code is governed by the MIT license that can be
 * found in the LICENSE file.
 */

#include "movegenerator.h"
#include "notation.h"

#include "gtest/gtest.h"

using namespace pulse;

class E {
public:
	int depth;
	uint64_t nodes;

	E(int depth, uint64_t nodes) : depth(depth), nodes(nodes) {}
};

class P {
public:
	std::string fen;
	std::vector<E> perftEntries;

	P(std::string fen, std::initializer_list<E> perftEntries) : fen(fen), perftEntries(perftEntries) {}
};

static const std::vector<P> perftPositions = {
		P("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
				{E(1, 20), E(2, 400), E(3, 8902), E(4, 197281), E(5, 4865609), E(6, 119060324)}),
		P("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
				{E(1, 48), E(2, 2039), E(3, 97862), E(4, 4085603), E(5, 193690690)}),
		P("4k3/8/8/8/8/8/8/4K2R w K - 0 1", {E(1, 15), E(2, 66), E(3, 1197), E(4, 7059), E(5, 133987), E(6, 764643)}),
		P("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1", {E(1, 16), E(2, 71), E(3, 1287), E(4, 7626), E(5, 145232), E(6, 846648)}),
		P("4k2r/8/8/8/8/8/8/4K3 w k - 0 1", {E(1, 5), E(2, 75), E(3, 459), E(4, 8290), E(5, 47635), E(6, 899442)}),
		P("r3k3/8/8/8/8/8/8/4K3 w q - 0 1", {E(1, 5), E(2, 80), E(3, 493), E(4, 8897), E(5, 52710), E(6, 1001523)}),
		P("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1",
				{E(1, 26), E(2, 112), E(3, 3189), E(4, 17945), E(5, 532933), E(6, 2788982)}),
		P("r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1",
				{E(1, 5), E(2, 130), E(3, 782), E(4, 22180), E(5, 118882), E(6, 3517770)}),
		P("8/8/8/8/8/8/6k1/4K2R w K - 0 1", {E(1, 12), E(2, 38), E(3, 564), E(4, 2219), E(5, 37735), E(6, 185867)}),
		P("8/8/8/8/8/8/1k6/R3K3 w Q - 0 1", {E(1, 15), E(2, 65), E(3, 1018), E(4, 4573), E(5, 80619), E(6, 413018)}),
		P("4k2r/6K1/8/8/8/8/8/8 w k - 0 1", {E(1, 3), E(2, 32), E(3, 134), E(4, 2073), E(5, 10485), E(6, 179869)}),
		P("r3k3/1K6/8/8/8/8/8/8 w q - 0 1", {E(1, 4), E(2, 49), E(3, 243), E(4, 3991), E(5, 20780), E(6, 367724)}),
		P("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1",
				{E(1, 26), E(2, 568), E(3, 13744), E(4, 314346), E(5, 7594526), E(6, 179862938)}),
		P("r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1",
				{E(1, 25), E(2, 567), E(3, 14095), E(4, 328965), E(5, 8153719), E(6, 195629489)}),
		P("r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1",
				{E(1, 25), E(2, 548), E(3, 13502), E(4, 312835), E(5, 7736373), E(6, 184411439)}),
		P("r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1",
				{E(1, 25), E(2, 547), E(3, 13579), E(4, 316214), E(5, 7878456), E(6, 189224276)}),
		P("1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1",
				{E(1, 26), E(2, 583), E(3, 14252), E(4, 334705), E(5, 8198901), E(6, 198328929)}),
		P("2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1",
				{E(1, 25), E(2, 560), E(3, 13592), E(4, 317324), E(5, 7710115), E(6, 185959088)}),
		P("r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1",
				{E(1, 25), E(2, 560), E(3, 13607), E(4, 320792), E(5, 7848606), E(6, 190755813)}),
		P("4k3/8/8/8/8/8/8/4K2R b K - 0 1", {E(1, 5), E(2, 75), E(3, 459), E(4, 8290), E(5, 47635), E(6, 899442)}),
		P("4k3/8/8/8/8/8/8/R3K3 b Q - 0 1", {E(1, 5), E(2, 80), E(3, 493), E(4, 8897), E(5, 52710), E(6, 1001523)}),
		P("4k2r/8/8/8/8/8/8/4K3 b k - 0 1", {E(1, 15), E(2, 66), E(3, 1197), E(4, 7059), E(5, 133987), E(6, 764643)}),
		P("r3k3/8/8/8/8/8/8/4K3 b q - 0 1", {E(1, 16), E(2, 71), E(3, 1287), E(4, 7626), E(5, 145232), E(6, 846648)}),
		P("4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1",
				{E(1, 5), E(2, 130), E(3, 782), E(4, 22180), E(5, 118882), E(6, 3517770)}),
		P("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1",
				{E(1, 26), E(2, 112), E(3, 3189), E(4, 17945), E(5, 532933), E(6, 2788982)}),
		P("8/8/8/8/8/8/6k1/4K2R b K - 0 1", {E(1, 3), E(2, 32), E(3, 134), E(4, 2073), E(5, 10485), E(6, 179869)}),
		P("8/8/8/8/8/8/1k6/R3K3 b Q - 0 1", {E(1, 4), E(2, 49), E(3, 243), E(4, 3991), E(5, 20780), E(6, 367724)}),
		P("4k2r/6K1/8/8/8/8/8/8 b k - 0 1", {E(1, 12), E(2, 38), E(3, 564), E(4, 2219), E(5, 37735), E(6, 185867)}),
		P("r3k3/1K6/8/8/8/8/8/8 b q - 0 1", {E(1, 15), E(2, 65), E(3, 1018), E(4, 4573), E(5, 80619), E(6, 413018)}),
		P("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1",
				{E(1, 26), E(2, 568), E(3, 13744), E(4, 314346), E(5, 7594526), E(6, 179862938)}),
		P("r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1",
				{E(1, 26), E(2, 583), E(3, 14252), E(4, 334705), E(5, 8198901), E(6, 198328929)}),
		P("r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1",
				{E(1, 25), E(2, 560), E(3, 13592), E(4, 317324), E(5, 7710115), E(6, 185959088)}),
		P("r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1",
				{E(1, 25), E(2, 560), E(3, 13607), E(4, 320792), E(5, 7848606), E(6, 190755813)}),
		P("1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1",
				{E(1, 25), E(2, 567), E(3, 14095), E(4, 328965), E(5, 8153719), E(6, 195629489)}),
		P("2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1",
				{E(1, 25), E(2, 548), E(3, 13502), E(4, 312835), E(5, 7736373), E(6, 184411439)}),
		P("r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1",
				{E(1, 25), E(2, 547), E(3, 13579), E(4, 316214), E(5, 7878456), E(6, 189224276)}),
		P("8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1",
				{E(1, 14), E(2, 195), E(3, 2760), E(4, 38675), E(5, 570726), E(6, 8107539)}),
		P("8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1",
				{E(1, 11), E(2, 156), E(3, 1636), E(4, 20534), E(5, 223507), E(6, 2594412)}),
		P("8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1",
				{E(1, 19), E(2, 289), E(3, 4442), E(4, 73584), E(5, 1198299), E(6, 19870403)}),
		P("K7/8/2n5/1n6/8/8/8/k6N w - - 0 1", {E(1, 3), E(2, 51), E(3, 345), E(4, 5301), E(5, 38348), E(6, 588695)}),
		P("k7/8/2N5/1N6/8/8/8/K6n w - - 0 1", {E(1, 17), E(2, 54), E(3, 835), E(4, 5910), E(5, 92250), E(6, 688780)}),
		P("8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1",
				{E(1, 15), E(2, 193), E(3, 2816), E(4, 40039), E(5, 582642), E(6, 8503277)}),
		P("8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1",
				{E(1, 16), E(2, 180), E(3, 2290), E(4, 24640), E(5, 288141), E(6, 3147566)}),
		P("8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1",
				{E(1, 4), E(2, 68), E(3, 1118), E(4, 16199), E(5, 281190), E(6, 4405103)}),
		P("K7/8/2n5/1n6/8/8/8/k6N b - - 0 1", {E(1, 17), E(2, 54), E(3, 835), E(4, 5910), E(5, 92250), E(6, 688780)}),
		P("k7/8/2N5/1N6/8/8/8/K6n b - - 0 1", {E(1, 3), E(2, 51), E(3, 345), E(4, 5301), E(5, 38348), E(6, 588695)}),
		P("B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1",
				{E(1, 17), E(2, 278), E(3, 4607), E(4, 76778), E(5, 1320507), E(6, 22823890)}),
		P("8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1",
				{E(1, 21), E(2, 316), E(3, 5744), E(4, 93338), E(5, 1713368), E(6, 28861171)}),
		P("k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1",
				{E(1, 21), E(2, 144), E(3, 3242), E(4, 32955), E(5, 787524), E(6, 7881673)}),
		P("K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1",
				{E(1, 7), E(2, 143), E(3, 1416), E(4, 31787), E(5, 310862), E(6, 7382896)}),
		P("B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1",
				{E(1, 6), E(2, 106), E(3, 1829), E(4, 31151), E(5, 530585), E(6, 9250746)}),
		P("8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1",
				{E(1, 17), E(2, 309), E(3, 5133), E(4, 93603), E(5, 1591064), E(6, 29027891)}),
		P("k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1",
				{E(1, 7), E(2, 143), E(3, 1416), E(4, 31787), E(5, 310862), E(6, 7382896)}),
		P("K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1",
				{E(1, 21), E(2, 144), E(3, 3242), E(4, 32955), E(5, 787524), E(6, 7881673)}),
		P("7k/RR6/8/8/8/8/rr6/7K w - - 0 1",
				{E(1, 19), E(2, 275), E(3, 5300), E(4, 104342), E(5, 2161211), E(6, 44956585)}),
		P("R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1",
				{E(1, 36), E(2, 1027), E(3, 29215), E(4, 771461), E(5, 20506480), E(6, 525169084)}),
		P("7k/RR6/8/8/8/8/rr6/7K b - - 0 1",
				{E(1, 19), E(2, 275), E(3, 5300), E(4, 104342), E(5, 2161211), E(6, 44956585)}),
		P("R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1",
				{E(1, 36), E(2, 1027), E(3, 29227), E(4, 771368), E(5, 20521342), E(6, 524966748)}),
		P("6kq/8/8/8/8/8/8/7K w - - 0 1", {E(1, 2), E(2, 36), E(3, 143), E(4, 3637), E(5, 14893), E(6, 391507)}),
		P("K7/8/8/3Q4/4q3/8/8/7k w - - 0 1", {E(1, 6), E(2, 35), E(3, 495), E(4, 8349), E(5, 166741), E(6, 3370175)}),
		P("6qk/8/8/8/8/8/8/7K b - - 0 1", {E(1, 22), E(2, 43), E(3, 1015), E(4, 4167), E(5, 105749), E(6, 419369)}),
		P("6KQ/8/8/8/8/8/8/7k b - - 0 1", {E(1, 2), E(2, 36), E(3, 143), E(4, 3637), E(5, 14893), E(6, 391507)}),
		P("K7/8/8/3Q4/4q3/8/8/7k b - - 0 1", {E(1, 6), E(2, 35), E(3, 495), E(4, 8349), E(5, 166741), E(6, 3370175)}),
		P("8/8/8/8/8/K7/P7/k7 w - - 0 1", {E(1, 3), E(2, 7), E(3, 43), E(4, 199), E(5, 1347), E(6, 6249)}),
		P("8/8/8/8/8/7K/7P/7k w - - 0 1", {E(1, 3), E(2, 7), E(3, 43), E(4, 199), E(5, 1347), E(6, 6249)}),
		P("K7/p7/k7/8/8/8/8/8 w - - 0 1", {E(1, 1), E(2, 3), E(3, 12), E(4, 80), E(5, 342), E(6, 2343)}),
		P("7K/7p/7k/8/8/8/8/8 w - - 0 1", {E(1, 1), E(2, 3), E(3, 12), E(4, 80), E(5, 342), E(6, 2343)}),
		P("8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1", {E(1, 7), E(2, 35), E(3, 210), E(4, 1091), E(5, 7028), E(6, 34834)}),
		P("8/8/8/8/8/K7/P7/k7 b - - 0 1", {E(1, 1), E(2, 3), E(3, 12), E(4, 80), E(5, 342), E(6, 2343)}),
		P("8/8/8/8/8/7K/7P/7k b - - 0 1", {E(1, 1), E(2, 3), E(3, 12), E(4, 80), E(5, 342), E(6, 2343)}),
		P("K7/p7/k7/8/8/8/8/8 b - - 0 1", {E(1, 3), E(2, 7), E(3, 43), E(4, 199), E(5, 1347), E(6, 6249)}),
		P("7K/7p/7k/8/8/8/8/8 b - - 0 1", {E(1, 3), E(2, 7), E(3, 43), E(4, 199), E(5, 1347), E(6, 6249)}),
		P("8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1", {E(1, 5), E(2, 35), E(3, 182), E(4, 1091), E(5, 5408), E(6, 34822)}),
		P("8/8/8/8/8/4k3/4P3/4K3 w - - 0 1", {E(1, 2), E(2, 8), E(3, 44), E(4, 282), E(5, 1814), E(6, 11848)}),
		P("4k3/4p3/4K3/8/8/8/8/8 b - - 0 1", {E(1, 2), E(2, 8), E(3, 44), E(4, 282), E(5, 1814), E(6, 11848)}),
		P("8/8/7k/7p/7P/7K/8/8 w - - 0 1", {E(1, 3), E(2, 9), E(3, 57), E(4, 360), E(5, 1969), E(6, 10724)}),
		P("8/8/k7/p7/P7/K7/8/8 w - - 0 1", {E(1, 3), E(2, 9), E(3, 57), E(4, 360), E(5, 1969), E(6, 10724)}),
		P("8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1", {E(1, 5), E(2, 25), E(3, 180), E(4, 1294), E(5, 8296), E(6, 53138)}),
		P("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1", {E(1, 8), E(2, 61), E(3, 483), E(4, 3213), E(5, 23599), E(6, 157093)}),
		P("8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1", {E(1, 8), E(2, 61), E(3, 411), E(4, 3213), E(5, 21637), E(6, 158065)}),
		P("k7/8/3p4/8/3P4/8/8/7K w - - 0 1", {E(1, 4), E(2, 15), E(3, 90), E(4, 534), E(5, 3450), E(6, 20960)}),
		P("8/8/7k/7p/7P/7K/8/8 b - - 0 1", {E(1, 3), E(2, 9), E(3, 57), E(4, 360), E(5, 1969), E(6, 10724)}),
		P("8/8/k7/p7/P7/K7/8/8 b - - 0 1", {E(1, 3), E(2, 9), E(3, 57), E(4, 360), E(5, 1969), E(6, 10724)}),
		P("8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1", {E(1, 5), E(2, 25), E(3, 180), E(4, 1294), E(5, 8296), E(6, 53138)}),
		P("8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1", {E(1, 8), E(2, 61), E(3, 411), E(4, 3213), E(5, 21637), E(6, 158065)}),
		P("8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1", {E(1, 8), E(2, 61), E(3, 483), E(4, 3213), E(5, 23599), E(6, 157093)}),
		P("k7/8/3p4/8/3P4/8/8/7K b - - 0 1", {E(1, 4), E(2, 15), E(3, 89), E(4, 537), E(5, 3309), E(6, 21104)}),
		P("7k/3p4/8/8/3P4/8/8/K7 w - - 0 1", {E(1, 4), E(2, 19), E(3, 117), E(4, 720), E(5, 4661), E(6, 32191)}),
		P("7k/8/8/3p4/8/8/3P4/K7 w - - 0 1", {E(1, 5), E(2, 19), E(3, 116), E(4, 716), E(5, 4786), E(6, 30980)}),
		P("k7/8/8/7p/6P1/8/8/K7 w - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("k7/8/7p/8/8/6P1/8/K7 w - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("k7/8/8/6p1/7P/8/8/K7 w - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("k7/8/6p1/8/8/7P/8/K7 w - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("k7/8/8/3p4/4p3/8/8/7K w - - 0 1", {E(1, 3), E(2, 15), E(3, 84), E(4, 573), E(5, 3013), E(6, 22886)}),
		P("k7/8/3p4/8/8/4P3/8/7K w - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4271), E(6, 28662)}),
		P("7k/3p4/8/8/3P4/8/8/K7 b - - 0 1", {E(1, 5), E(2, 19), E(3, 117), E(4, 720), E(5, 5014), E(6, 32167)}),
		P("7k/8/8/3p4/8/8/3P4/K7 b - - 0 1", {E(1, 4), E(2, 19), E(3, 117), E(4, 712), E(5, 4658), E(6, 30749)}),
		P("k7/8/8/7p/6P1/8/8/K7 b - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("k7/8/7p/8/8/6P1/8/K7 b - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("k7/8/8/6p1/7P/8/8/K7 b - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("k7/8/6p1/8/8/7P/8/K7 b - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("k7/8/8/3p4/4p3/8/8/7K b - - 0 1", {E(1, 5), E(2, 15), E(3, 102), E(4, 569), E(5, 4337), E(6, 22579)}),
		P("k7/8/3p4/8/8/4P3/8/7K b - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4271), E(6, 28662)}),
		P("7k/8/8/p7/1P6/8/8/7K w - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("7k/8/p7/8/8/1P6/8/7K w - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("7k/8/8/1p6/P7/8/8/7K w - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("7k/8/1p6/8/8/P7/8/7K w - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("k7/7p/8/8/8/8/6P1/K7 w - - 0 1", {E(1, 5), E(2, 25), E(3, 161), E(4, 1035), E(5, 7574), E(6, 55338)}),
		P("k7/6p1/8/8/8/8/7P/K7 w - - 0 1", {E(1, 5), E(2, 25), E(3, 161), E(4, 1035), E(5, 7574), E(6, 55338)}),
		P("3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1", {E(1, 7), E(2, 49), E(3, 378), E(4, 2902), E(5, 24122), E(6, 199002)}),
		P("7k/8/8/p7/1P6/8/8/7K b - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("7k/8/p7/8/8/1P6/8/7K b - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("7k/8/8/1p6/P7/8/8/7K b - - 0 1", {E(1, 5), E(2, 22), E(3, 139), E(4, 877), E(5, 6112), E(6, 41874)}),
		P("7k/8/1p6/8/8/P7/8/7K b - - 0 1", {E(1, 4), E(2, 16), E(3, 101), E(4, 637), E(5, 4354), E(6, 29679)}),
		P("k7/7p/8/8/8/8/6P1/K7 b - - 0 1", {E(1, 5), E(2, 25), E(3, 161), E(4, 1035), E(5, 7574), E(6, 55338)}),
		P("k7/6p1/8/8/8/8/7P/K7 b - - 0 1", {E(1, 5), E(2, 25), E(3, 161), E(4, 1035), E(5, 7574), E(6, 55338)}),
		P("3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1", {E(1, 7), E(2, 49), E(3, 378), E(4, 2902), E(5, 24122), E(6, 199002)}),
		P("8/Pk6/8/8/8/8/6Kp/8 w - - 0 1", {E(1, 11), E(2, 97), E(3, 887), E(4, 8048), E(5, 90606), E(6, 1030499)}),
		P("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1",
				{E(1, 24), E(2, 421), E(3, 7421), E(4, 124608), E(5, 2193768), E(6, 37665329)}),
		P("8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1",
				{E(1, 18), E(2, 270), E(3, 4699), E(4, 79355), E(5, 1533145), E(6, 28859283)}),
		P("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1",
				{E(1, 24), E(2, 496), E(3, 9483), E(4, 182838), E(5, 3605103), E(6, 71179139)}),
		P("8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", {E(1, 11), E(2, 97), E(3, 887), E(4, 8048), E(5, 90606), E(6, 1030499)}),
		P("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1",
				{E(1, 24), E(2, 421), E(3, 7421), E(4, 124608), E(5, 2193768), E(6, 37665329)}),
		P("8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1",
				{E(1, 18), E(2, 270), E(3, 4699), E(4, 79355), E(5, 1533145), E(6, 28859283)}),
		P("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1",
				{E(1, 24), E(2, 496), E(3, 9483), E(4, 182838), E(5, 3605103), E(6, 71179139)}),
		P("1k6/8/8/5pP1/4K1P1/8/8/8 w - f6 0 1", {E(1, 10), E(2, 63), E(3, 533), E(4, 3508), E(5, 30821)})
};

static const int MAX_DEPTH = 6;
static std::array<MoveGenerator, MAX_DEPTH> moveGenerators;

uint64_t miniMax(int depth, Position& position, int ply) {
	if (depth <= 0) {
		return 1;
	}

	uint64_t totalNodes = 0;

	bool isCheck = position.isCheck();
	MoveList<MoveEntry>& moves = moveGenerators[ply].getMoves(position, depth, isCheck);

	for (int i = 0; i < moves.size; i++) {
		int move = moves.entries[i]->move;

		position.makeMove(move);
		if (!position.isCheck(Color::opposite(position.activeColor))) {
			totalNodes += miniMax(depth - 1, position, ply + 1);
		}
		position.undoMove(move);
	}

	return totalNodes;
}

TEST(movegeneratortest, testPerft) {
	for (unsigned int i = 0; i < 4; i++) {
		for (const auto& p : perftPositions) {
			if (p.perftEntries.size() > i) {
				int depth = p.perftEntries[i].depth;
				uint64_t nodes = p.perftEntries[i].nodes;

				Position position(Notation::toPosition(p.fen));

				uint64_t result = miniMax(depth, position, 0);
				EXPECT_EQ(nodes, result)
									<< Notation::fromPosition(position) << ", depth " << i
									<< ": expected " << nodes
									<< ", actual " << result;
			}
		}
	}
}
