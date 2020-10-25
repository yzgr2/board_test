import numpy as np
import scipy.fftpack as fftpack
import matplotlib.pyplot as plt
from matplotlib.pylab import mpl

import sys

# sig_mode=0, Rate=9, RSSI=-53 cwb=0 stbc=0 datlen=128 firstword_invalid=1 channel=4
dat = [ 
        
[ 31 + (1j) , 27 + (2j) , 21 + (-2j) , 24 + (1j) , 7 + (2j) , 13 + (-5j) , 12 + (-7j) , 5 + (-6j) , 9 + (-14j) , 3 + (-25j) , 7 + (-24j) , 1 + (-28j) , 9 + (-22j) , 10 + (-27j) , 9 + (-25j) , 10 + (-26j) , 8 + (-30j) , 10 + (-22j) , 4 + (-17j) , 6 + (-16j) , 3 + (-14j) , -4 + (-12j) , -11 + (-10j) , -13 + (-11j) , -12 + (-5j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -8 + (1j) , 0 + (4j) , -6 + (3j) , 6 + (4j) , 8 + (7j) , 10 + (12j) , 19 + (5j) , 27 + (17j) , 21 + (17j) , 17 + (16j) , 15 + (10j) , 21 + (20j) , 12 + (18j) , 5 + (22j) , 11 + (18j) , 9 + (13j) , 15 + (8j) , 8 + (2j) , 13 + (1j) , 9 + (-4j) , 13 + (-3j) , 13 + (1j) , 20 + (-11j) , 22 + (-14j) , 26 + (-9j) , 32 + (-4j) ,],


[ 11 + (11j) , 16 + (10j) , 17 + (5j) , 16 + (3j) , 16 + (4j) , 22 + (8j) , 20 + (8j) , 24 + (8j) , 28 + (15j) , 18 + (12j) , 19 + (14j) , 17 + (21j) , 15 + (15j) , 14 + (14j) , 7 + (17j) , 8 + (9j) , 10 + (8j) , 15 + (5j) , 8 + (-2j) , 17 + (-5j) , 15 + (-7j) , 20 + (-10j) , 24 + (-12j) , 20 + (-12j) , 20 + (-12j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -1 + (22j) , 3 + (21j) , -3 + (21j) , 6 + (24j) , 6 + (22j) , 5 + (19j) , 2 + (22j) , 4 + (23j) , 9 + (21j) , 1 + (20j) , 5 + (19j) , 2 + (13j) , 8 + (13j) , 6 + (10j) , 14 + (13j) , 15 + (8j) , 14 + (13j) , 22 + (9j) , 21 + (16j) , 23 + (19j) , 23 + (19j) , 19 + (25j) , 15 + (23j) , 16 + (24j) , 13 + (18j) , 8 + (17j) ,],


[ -10 + (-16j) , -11 + (-13j) , -7 + (-15j) , -10 + (-14j) , -8 + (-23j) , 2 + (-25j) , -7 + (-20j) , -7 + (-23j) , -5 + (-17j) , -4 + (-22j) , -4 + (-21j) , 0 + (-20j) , 5 + (-11j) , 3 + (-17j) , 9 + (-12j) , 3 + (-12j) , 4 + (-13j) , 9 + (-6j) , 11 + (-5j) , 7 + (-7j) , 9 + (-2j) , 8 + (2j) , 15 + (-5j) , 11 + (-2j) , 10 + (-1j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -4 + (23j) , 4 + (31j) , 8 + (35j) , 11 + (33j) , 15 + (26j) , 10 + (29j) , 18 + (19j) , 16 + (19j) , 15 + (14j) , 9 + (10j) , 6 + (15j) , -1 + (6j) , -5 + (-2j) , -6 + (7j) , -15 + (3j) , -18 + (8j) , -29 + (9j) , -23 + (10j) , -27 + (11j) , -28 + (15j) , -23 + (15j) , -26 + (10j) , -23 + (7j) , -17 + (9j) , -22 + (10j) , -13 + (1j) ,],


[ -10 + (13j) , -6 + (15j) , -11 + (6j) , -13 + (8j) , -12 + (-6j) , -10 + (-7j) , -24 + (-4j) , -25 + (-2j) , -20 + (4j) , -18 + (-15j) , -22 + (-4j) , -20 + (4j) , -27 + (0j) , -20 + (-4j) , -17 + (-4j) , -10 + (1j) , -16 + (0j) , -11 + (-2j) , -9 + (-5j) , -9 + (-4j) , 1 + (-10j) , -5 + (-17j) , -6 + (-11j) , 5 + (-16j) , -11 + (-18j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 4 + (14j) , 2 + (20j) , 3 + (22j) , 1 + (18j) , 9 + (26j) , 10 + (25j) , 18 + (17j) , 15 + (30j) , 7 + (26j) , 20 + (23j) , 12 + (17j) , 13 + (21j) , 18 + (7j) , 9 + (14j) , 9 + (11j) , 4 + (15j) , -8 + (15j) , -6 + (19j) , -15 + (12j) , -9 + (6j) , -11 + (26j) , -10 + (20j) , -11 + (26j) , -11 + (27j) , -10 + (26j) , -8 + (25j) ,],


[ -2 + (-3j) , 1 + (-7j) , 8 + (-10j) , 8 + (-10j) , 6 + (-15j) , 14 + (-14j) , 13 + (-15j) , 6 + (-13j) , 15 + (-21j) , 7 + (-19j) , 2 + (-15j) , 9 + (-19j) , 9 + (-16j) , 5 + (-15j) , -9 + (-8j) , -9 + (-6j) , -7 + (1j) , -8 + (-1j) , -6 + (8j) , -14 + (10j) , -6 + (16j) , -8 + (21j) , -2 + (17j) , 7 + (17j) , 10 + (22j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 32 + (20j) , 21 + (15j) , 25 + (10j) , 22 + (15j) , 19 + (15j) , 18 + (11j) , 17 + (15j) , 14 + (17j) , 12 + (12j) , 4 + (14j) , -2 + (12j) , -8 + (6j) , -15 + (19j) , -10 + (15j) , -15 + (16j) , -16 + (11j) , -16 + (15j) , -23 + (13j) , -27 + (6j) , -22 + (12j) , -28 + (10j) , -21 + (6j) , -18 + (10j) , -14 + (2j) , -11 + (4j) , -12 + (7j) ,],


[ -19 + (14j) , -24 + (17j) , -22 + (9j) , -17 + (13j) , -23 + (7j) , -20 + (7j) , -14 + (10j) , -10 + (11j) , -15 + (17j) , -18 + (20j) , -10 + (19j) , -10 + (22j) , -15 + (26j) , -13 + (21j) , -21 + (27j) , -20 + (30j) , -20 + (28j) , -16 + (19j) , -25 + (22j) , -15 + (24j) , -21 + (22j) , -17 + (12j) , -3 + (18j) , 2 + (8j) , 7 + (14j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -24 + (2j) , -25 + (0j) , -27 + (4j) , -23 + (-4j) , -21 + (-7j) , -23 + (1j) , -24 + (3j) , -23 + (-7j) , -24 + (-4j) , -17 + (0j) , -19 + (-8j) , -14 + (-2j) , -10 + (-3j) , -5 + (4j) , -12 + (12j) , -12 + (8j) , -18 + (9j) , -16 + (11j) , -15 + (10j) , -9 + (18j) , -13 + (15j) , -20 + (17j) , -27 + (7j) , -22 + (10j) , -24 + (15j) , -30 + (14j) ,],


[ -19 + (17j) , -21 + (10j) , -18 + (11j) , -14 + (14j) , -16 + (8j) , -7 + (14j) , -20 + (20j) , -13 + (13j) , -11 + (20j) , -15 + (19j) , -8 + (20j) , -9 + (26j) , -18 + (29j) , -19 + (28j) , -12 + (28j) , -13 + (30j) , -16 + (29j) , -13 + (20j) , -16 + (20j) , -9 + (18j) , -11 + (16j) , -4 + (25j) , 2 + (22j) , 3 + (20j) , 15 + (16j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -12 + (9j) , -16 + (16j) , -15 + (9j) , -16 + (12j) , -19 + (14j) , -20 + (6j) , -24 + (7j) , -24 + (6j) , -24 + (2j) , -21 + (3j) , -15 + (-4j) , -17 + (-3j) , -17 + (2j) , -16 + (12j) , -17 + (12j) , -7 + (16j) , -8 + (15j) , -7 + (15j) , -12 + (17j) , -13 + (20j) , -18 + (20j) , -8 + (22j) , -12 + (18j) , -17 + (22j) , -28 + (16j) , -24 + (14j) ,],


[ 10 + (27j) , 8 + (32j) , 3 + (34j) , -1 + (23j) , -2 + (17j) , 8 + (18j) , 9 + (9j) , 11 + (11j) , 10 + (2j) , 19 + (3j) , 22 + (4j) , 20 + (6j) , 22 + (9j) , 32 + (11j) , 26 + (21j) , 28 + (18j) , 29 + (20j) , 19 + (16j) , 20 + (22j) , 26 + (12j) , 20 + (6j) , 12 + (6j) , 10 + (0j) , 11 + (-6j) , 15 + (-14j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -5 + (2j) , -2 + (13j) , -4 + (11j) , 0 + (18j) , 1 + (13j) , -3 + (17j) , -5 + (17j) , -1 + (19j) , -2 + (22j) , 3 + (16j) , -8 + (20j) , 2 + (16j) , -8 + (19j) , -2 + (20j) , 2 + (17j) , -2 + (10j) , 3 + (15j) , 1 + (6j) , 5 + (6j) , 5 + (12j) , 10 + (14j) , 15 + (14j) , 20 + (18j) , 21 + (17j) , 19 + (23j) , 12 + (21j) ,],


[ -30 + (1j) , -31 + (0j) , -27 + (-4j) , -23 + (-5j) , -19 + (-2j) , -18 + (-1j) , -12 + (8j) , -11 + (5j) , -9 + (10j) , -14 + (20j) , -7 + (20j) , -7 + (24j) , -8 + (28j) , -15 + (29j) , -16 + (30j) , -16 + (25j) , -15 + (29j) , -20 + (24j) , -20 + (27j) , -16 + (11j) , -10 + (12j) , -9 + (15j) , 2 + (12j) , 3 + (9j) , 8 + (8j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -6 + (-11j) , -6 + (-14j) , -9 + (-9j) , -13 + (-3j) , -14 + (-16j) , -17 + (-12j) , -17 + (-17j) , -15 + (-18j) , -16 + (-12j) , -17 + (-14j) , -20 + (-16j) , -17 + (-14j) , -13 + (-19j) , -17 + (-8j) , -15 + (-8j) , -12 + (-8j) , -11 + (-7j) , -14 + (-1j) , -13 + (-1j) , -15 + (4j) , -17 + (0j) , -14 + (7j) , -24 + (6j) , -23 + (8j) , -27 + (16j) , -32 + (6j) ,],


[ -2 + (25j) , -11 + (20j) , 1 + (16j) , 4 + (8j) , 7 + (9j) , 4 + (14j) , 17 + (3j) , 9 + (6j) , 20 + (6j) , 21 + (5j) , 20 + (15j) , 19 + (20j) , 22 + (24j) , 23 + (18j) , 25 + (15j) , 19 + (17j) , 17 + (19j) , 9 + (15j) , 9 + (14j) , 5 + (7j) , 4 + (4j) , 1 + (-4j) , 3 + (-5j) , 1 + (-15j) , 6 + (-21j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -12 + (3j) , -11 + (17j) , -20 + (12j) , -18 + (14j) , -16 + (15j) , -18 + (14j) , -23 + (11j) , -22 + (18j) , -24 + (18j) , -23 + (20j) , -21 + (8j) , -16 + (11j) , -13 + (10j) , -8 + (9j) , -6 + (19j) , 7 + (14j) , 7 + (11j) , 1 + (12j) , 2 + (20j) , 6 + (20j) , 12 + (27j) , 5 + (25j) , 7 + (24j) , 3 + (26j) , 5 + (32j) , 3 + (36j) ,],


[ -17 + (14j) , -18 + (16j) , -18 + (9j) , -9 + (9j) , -10 + (7j) , 2 + (14j) , 3 + (17j) , 8 + (14j) , 9 + (18j) , 9 + (14j) , 14 + (26j) , 10 + (23j) , 5 + (28j) , 5 + (32j) , 3 + (30j) , 3 + (28j) , 2 + (29j) , -5 + (22j) , -4 + (17j) , 0 + (14j) , 2 + (12j) , 3 + (0j) , 5 + (-2j) , 9 + (-5j) , 7 + (-14j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -9 + (-11j) , -17 + (-8j) , -23 + (-8j) , -18 + (-9j) , -29 + (-7j) , -26 + (-4j) , -30 + (-13j) , -29 + (-6j) , -28 + (-12j) , -23 + (-11j) , -22 + (-6j) , -20 + (-5j) , -16 + (-2j) , -10 + (-2j) , -19 + (3j) , -18 + (7j) , -13 + (7j) , -10 + (6j) , -9 + (13j) , -11 + (20j) , -11 + (28j) , -17 + (21j) , -19 + (22j) , -16 + (14j) , -20 + (19j) , -16 + (13j) ,],


[ 3 + (29j) , -2 + (27j) , -2 + (21j) , -2 + (15j) , 3 + (16j) , 6 + (11j) , 4 + (8j) , 12 + (8j) , 19 + (8j) , 20 + (5j) , 21 + (14j) , 21 + (19j) , 22 + (13j) , 17 + (19j) , 20 + (27j) , 14 + (28j) , 17 + (27j) , 10 + (18j) , 11 + (17j) , 2 + (13j) , 6 + (14j) , 6 + (8j) , 1 + (-3j) , 0 + (0j) , -4 + (-2j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -9 + (24j) , -7 + (23j) , -8 + (23j) , -9 + (25j) , -12 + (19j) , -8 + (31j) , -13 + (22j) , -5 + (19j) , -8 + (16j) , -7 + (23j) , -8 + (16j) , -6 + (14j) , -3 + (20j) , 3 + (24j) , 3 + (16j) , 2 + (10j) , 5 + (14j) , 2 + (18j) , 12 + (20j) , 13 + (22j) , 20 + (20j) , 14 + (22j) , 10 + (20j) , 12 + (27j) , 11 + (29j) , 10 + (29j) ,],


[ -32 + (14j) , -28 + (14j) , -23 + (5j) , -14 + (4j) , -20 + (4j) , -10 + (0j) , -4 + (4j) , -7 + (4j) , -5 + (7j) , 3 + (24j) , 0 + (25j) , 1 + (31j) , -2 + (25j) , -4 + (28j) , -12 + (28j) , -15 + (33j) , -19 + (22j) , -14 + (22j) , -17 + (13j) , -17 + (11j) , -13 + (12j) , -5 + (8j) , -1 + (-4j) , 1 + (-8j) , 10 + (-5j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -4 + (-19j) , -5 + (-15j) , -8 + (-20j) , -8 + (-18j) , -9 + (-16j) , -11 + (-13j) , -7 + (-10j) , -16 + (-9j) , -16 + (-10j) , -17 + (-6j) , -16 + (-1j) , -22 + (-8j) , -20 + (-2j) , -21 + (-1j) , -23 + (5j) , -18 + (2j) , -24 + (-2j) , -18 + (0j) , -20 + (0j) , -12 + (4j) , -16 + (5j) , -20 + (11j) , -11 + (11j) , -20 + (15j) , -25 + (12j) , -20 + (18j) ,],


[ 18 + (23j) , 21 + (19j) , 15 + (17j) , 16 + (16j) , 8 + (5j) , 8 + (5j) , -2 + (-10j) , -1 + (-15j) , -1 + (-21j) , -4 + (-26j) , 0 + (-30j) , 3 + (-38j) , 6 + (-42j) , 7 + (-41j) , 8 + (-36j) , 6 + (-31j) , 11 + (-31j) , 12 + (-26j) , 5 + (-18j) , 2 + (-11j) , 9 + (-6j) , 3 + (2j) , -2 + (12j) , -6 + (18j) , -4 + (24j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 1 + (-14j) , -4 + (-6j) , -10 + (-8j) , -9 + (-4j) , -16 + (-6j) , -17 + (-3j) , -23 + (4j) , -24 + (8j) , -23 + (5j) , -26 + (5j) , -28 + (13j) , -25 + (16j) , -24 + (10j) , -22 + (7j) , -18 + (14j) , -20 + (8j) , -10 + (4j) , -4 + (13j) , 1 + (11j) , 8 + (10j) , 12 + (12j) , 17 + (15j) , 22 + (17j) , 27 + (18j) , 27 + (18j) , 34 + (23j) ,],


[ -23 + (-9j) , -25 + (-8j) , -25 + (-8j) , -15 + (-2j) , -9 + (-3j) , -7 + (-2j) , -1 + (12j) , 5 + (12j) , 11 + (14j) , 13 + (22j) , 14 + (28j) , 10 + (29j) , 10 + (31j) , 12 + (35j) , 5 + (34j) , 4 + (37j) , 4 + (32j) , -1 + (24j) , 3 + (21j) , 4 + (15j) , -2 + (5j) , -3 + (3j) , -7 + (0j) , -8 + (-14j) , -5 + (-20j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 5 + (0j) , 10 + (-4j) , 8 + (0j) , 18 + (-7j) , 18 + (-9j) , 24 + (-14j) , 19 + (-18j) , 17 + (-14j) , 11 + (-18j) , 15 + (-21j) , 20 + (-22j) , 16 + (-17j) , 18 + (-18j) , 12 + (-15j) , 10 + (-12j) , 4 + (-8j) , -3 + (-10j) , -4 + (-8j) , -8 + (-3j) , -13 + (-1j) , -27 + (-6j) , -28 + (-7j) , -28 + (-4j) , -37 + (-5j) , -37 + (-2j) , -34 + (-4j) ,],


[ 30 + (19j) , 23 + (16j) , 16 + (10j) , 12 + (5j) , 6 + (-2j) , 8 + (-4j) , -2 + (-4j) , -9 + (-11j) , -9 + (-16j) , -12 + (-23j) , -11 + (-30j) , -8 + (-31j) , -7 + (-40j) , -14 + (-40j) , -7 + (-36j) , -9 + (-33j) , -3 + (-39j) , -5 + (-29j) , 0 + (-30j) , -2 + (-21j) , 7 + (-11j) , 1 + (-6j) , -4 + (5j) , 5 + (7j) , 10 + (13j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (-5j) , -12 + (-3j) , -10 + (1j) , -25 + (-3j) , -20 + (0j) , -22 + (2j) , -23 + (2j) , -20 + (5j) , -27 + (6j) , -29 + (7j) , -24 + (14j) , -24 + (7j) , -19 + (8j) , -15 + (7j) , -13 + (4j) , -7 + (8j) , -1 + (14j) , 4 + (13j) , 4 + (5j) , 16 + (3j) , 16 + (11j) , 24 + (10j) , 27 + (15j) , 29 + (15j) , 34 + (16j) , 34 + (15j) ,],


[ -18 + (15j) , -15 + (12j) , -13 + (10j) , -6 + (11j) , -15 + (15j) , -2 + (12j) , -4 + (22j) , -6 + (19j) , -6 + (21j) , -7 + (30j) , -7 + (25j) , -7 + (24j) , -15 + (28j) , -13 + (28j) , -15 + (20j) , -13 + (16j) , -11 + (20j) , -14 + (14j) , -7 + (13j) , 1 + (12j) , -2 + (13j) , 3 + (6j) , 4 + (8j) , 12 + (10j) , 17 + (9j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -7 + (2j) , -16 + (5j) , -21 + (2j) , -18 + (5j) , -16 + (3j) , -19 + (2j) , -25 + (6j) , -22 + (4j) , -24 + (2j) , -17 + (6j) , -19 + (1j) , -18 + (2j) , -15 + (0j) , -14 + (2j) , -10 + (7j) , -7 + (9j) , -8 + (10j) , -10 + (13j) , -12 + (19j) , -13 + (15j) , -16 + (18j) , -19 + (19j) , -17 + (18j) , -14 + (19j) , -24 + (24j) , -21 + (15j) ,],


[ -18 + (-6j) , -19 + (-1j) , -17 + (1j) , -20 + (0j) , -19 + (0j) , -21 + (7j) , -21 + (9j) , -22 + (7j) , -19 + (5j) , -25 + (3j) , -24 + (3j) , -28 + (2j) , -31 + (4j) , -25 + (0j) , -20 + (-2j) , -15 + (0j) , -25 + (0j) , -24 + (-1j) , -20 + (1j) , -14 + (6j) , -9 + (5j) , -6 + (11j) , -10 + (12j) , -6 + (16j) , -2 + (21j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -17 + (-14j) , -24 + (-8j) , -21 + (-13j) , -24 + (-11j) , -25 + (-14j) , -24 + (-16j) , -23 + (-17j) , -20 + (-13j) , -18 + (-11j) , -19 + (-16j) , -13 + (-9j) , -15 + (-8j) , -8 + (-4j) , -11 + (3j) , -15 + (1j) , -15 + (5j) , -17 + (5j) , -23 + (6j) , -22 + (7j) , -24 + (2j) , -28 + (1j) , -26 + (-1j) , -26 + (-4j) , -27 + (-4j) , -27 + (-7j) , -28 + (-10j) ,],


[ -17 + (1j) , -21 + (4j) , -15 + (7j) , -10 + (6j) , -9 + (10j) , -10 + (8j) , -7 + (14j) , -16 + (14j) , -10 + (24j) , -8 + (25j) , -10 + (24j) , -6 + (27j) , -11 + (26j) , -10 + (24j) , -17 + (26j) , -12 + (24j) , -6 + (21j) , -10 + (16j) , -7 + (20j) , -1 + (9j) , 2 + (4j) , 1 + (5j) , 19 + (2j) , 14 + (-5j) , 11 + (-9j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -13 + (-4j) , -11 + (-11j) , -13 + (-12j) , -20 + (-16j) , -17 + (-10j) , -27 + (-24j) , -11 + (-20j) , -23 + (-14j) , -20 + (-25j) , -8 + (-18j) , -14 + (-22j) , -10 + (-24j) , -7 + (-13j) , -8 + (-8j) , -10 + (-5j) , -6 + (3j) , -12 + (1j) , -12 + (11j) , -12 + (6j) , -23 + (14j) , -24 + (5j) , -25 + (5j) , -28 + (7j) , -29 + (2j) , -21 + (-1j) , -18 + (-2j) ,],



[ 1 + (-28j) , -4 + (-13j) , -3 + (-14j) , 3 + (-21j) , 3 + (-10j) , 3 + (-8j) , -6 + (0j) , -9 + (3j) , -14 + (10j) , -16 + (19j) , -21 + (12j) , -24 + (20j) , -28 + (22j) , -25 + (16j) , -26 + (21j) , -26 + (16j) , -27 + (14j) , -23 + (12j) , -19 + (4j) , -19 + (4j) , -7 + (8j) , -7 + (4j) , 2 + (-3j) , 6 + (-2j) , 12 + (-12j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -9 + (7j) , -2 + (9j) , 6 + (10j) , 14 + (8j) , 21 + (4j) , 21 + (8j) , 24 + (3j) , 26 + (-1j) , 29 + (3j) , 25 + (6j) , 28 + (3j) , 32 + (5j) , 21 + (2j) , 20 + (2j) , 13 + (0j) , 13 + (4j) , 11 + (-4j) , 4 + (1j) , 0 + (-13j) , -4 + (-9j) , -8 + (-14j) , -8 + (-18j) , -8 + (-25j) , -9 + (-23j) , -10 + (-24j) , -4 + (-32j) ,],


[ -26 + (9j) , -23 + (-2j) , -18 + (5j) , -14 + (-2j) , -12 + (-3j) , 6 + (4j) , 3 + (-2j) , -2 + (0j) , 20 + (-5j) , 19 + (11j) , 18 + (13j) , 32 + (7j) , 25 + (13j) , 22 + (18j) , 23 + (15j) , 22 + (17j) , 26 + (14j) , 23 + (17j) , 18 + (14j) , 18 + (13j) , 15 + (-4j) , 10 + (1j) , -2 + (3j) , -16 + (-7j) , -21 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 6 + (8j) , 7 + (-3j) , 6 + (-10j) , 10 + (-12j) , 8 + (-14j) , 7 + (-19j) , 4 + (-25j) , 4 + (-25j) , 2 + (-26j) , 0 + (-23j) , 6 + (-22j) , 5 + (-26j) , 3 + (-21j) , 2 + (-18j) , 6 + (-17j) , -3 + (-6j) , -1 + (-4j) , -4 + (-5j) , -9 + (-1j) , -12 + (6j) , -8 + (5j) , -16 + (10j) , -24 + (15j) , -28 + (11j) , -24 + (4j) , -31 + (9j) ,],


[ 28 + (-15j) , 20 + (-13j) , 24 + (-13j) , 21 + (-10j) , 15 + (-8j) , 14 + (-7j) , 16 + (-6j) , 9 + (5j) , 7 + (7j) , 2 + (11j) , 8 + (19j) , 4 + (19j) , 5 + (23j) , 10 + (28j) , 8 + (27j) , 10 + (31j) , 13 + (25j) , 13 + (28j) , 14 + (25j) , 11 + (18j) , 9 + (14j) , 9 + (19j) , 7 + (9j) , 5 + (14j) , 7 + (12j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -7 + (-2j) , -8 + (0j) , -1 + (2j) , -2 + (-3j) , -7 + (-11j) , -6 + (-9j) , -7 + (-9j) , -8 + (-20j) , -8 + (-20j) , -16 + (-19j) , -19 + (-19j) , -12 + (-22j) , -15 + (-14j) , -18 + (-17j) , -13 + (-15j) , -13 + (-13j) , -17 + (-5j) , -6 + (-9j) , -3 + (-10j) , 2 + (-8j) , 8 + (-6j) , 12 + (-7j) , 19 + (-13j) , 19 + (-5j) , 21 + (-12j) , 24 + (-7j) ,],


[ 25 + (-16j) , 21 + (-10j) , 17 + (-9j) , 18 + (-7j) , 18 + (-9j) , 15 + (-8j) , 14 + (-2j) , 11 + (-2j) , 13 + (5j) , 12 + (12j) , 11 + (12j) , 12 + (13j) , 16 + (15j) , 18 + (16j) , 25 + (17j) , 21 + (17j) , 20 + (12j) , 27 + (10j) , 19 + (10j) , 17 + (9j) , 12 + (5j) , 18 + (3j) , 13 + (7j) , 14 + (8j) , 12 + (9j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -5 + (-11j) , -2 + (-7j) , -2 + (-7j) , -1 + (-10j) , 3 + (-11j) , 4 + (-15j) , 2 + (-15j) , 3 + (-16j) , 5 + (-15j) , 4 + (-20j) , 1 + (-26j) , -3 + (-16j) , 2 + (-22j) , 0 + (-21j) , -2 + (-19j) , 0 + (-14j) , -5 + (-16j) , -1 + (-13j) , 1 + (-12j) , 5 + (-5j) , 12 + (-9j) , 9 + (-9j) , 13 + (-3j) , 22 + (-3j) , 17 + (-10j) , 24 + (-10j) ,],


[ 18 + (-22j) , 23 + (-23j) , 12 + (-22j) , 17 + (-21j) , 16 + (-15j) , 18 + (-6j) , 12 + (-7j) , 11 + (-8j) , 15 + (7j) , 15 + (4j) , 16 + (7j) , 14 + (12j) , 24 + (11j) , 21 + (8j) , 26 + (5j) , 30 + (8j) , 29 + (9j) , 23 + (4j) , 26 + (0j) , 19 + (-1j) , 16 + (2j) , 16 + (2j) , 16 + (-1j) , 9 + (2j) , 17 + (8j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -5 + (-5j) , -5 + (-9j) , -6 + (-10j) , -8 + (-17j) , -4 + (-8j) , 5 + (-13j) , 1 + (-17j) , 2 + (-21j) , 2 + (-23j) , 2 + (-23j) , 0 + (-26j) , -1 + (-22j) , -9 + (-20j) , -11 + (-20j) , -9 + (-20j) , -4 + (-24j) , -8 + (-25j) , -3 + (-14j) , 5 + (-16j) , 1 + (-5j) , 4 + (-9j) , 10 + (-5j) , 14 + (-13j) , 22 + (-5j) , 17 + (-15j) , 22 + (-17j) ,],


[ -31 + (13j) , -27 + (14j) , -26 + (15j) , -25 + (9j) , -19 + (3j) , -21 + (-1j) , -26 + (-4j) , -15 + (-1j) , -15 + (-14j) , -14 + (-8j) , -13 + (-11j) , -17 + (-14j) , -24 + (-20j) , -17 + (-18j) , -25 + (-18j) , -26 + (-20j) , -29 + (-28j) , -27 + (-27j) , -27 + (-13j) , -28 + (-9j) , -24 + (-7j) , -23 + (-8j) , -26 + (-5j) , -21 + (-12j) , -13 + (-4j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 8 + (15j) , 8 + (6j) , 14 + (17j) , -5 + (0j) , -3 + (16j) , -7 + (2j) , -9 + (16j) , 0 + (15j) , -17 + (27j) , -3 + (27j) , -15 + (26j) , -11 + (29j) , 3 + (27j) , 1 + (21j) , 3 + (27j) , -7 + (21j) , 4 + (11j) , -6 + (9j) , -8 + (12j) , -17 + (2j) , -14 + (4j) , -11 + (4j) , -20 + (-3j) , -34 + (7j) , -28 + (4j) , -35 + (2j) ,],


[ 26 + (7j) , 21 + (6j) , 18 + (9j) , 18 + (10j) , 11 + (9j) , 9 + (7j) , 7 + (9j) , 7 + (9j) , -1 + (13j) , 3 + (10j) , 2 + (16j) , -5 + (21j) , -10 + (23j) , -2 + (23j) , 2 + (23j) , 1 + (24j) , 0 + (24j) , 3 + (22j) , 4 + (17j) , 4 + (24j) , 2 + (19j) , 5 + (8j) , 2 + (14j) , 2 + (14j) , 2 + (15j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 1 + (-17j) , 1 + (-4j) , 5 + (-6j) , 6 + (-6j) , 7 + (-4j) , 6 + (-7j) , 17 + (-2j) , 13 + (-8j) , 20 + (-11j) , 18 + (-11j) , 13 + (-15j) , 14 + (-19j) , 9 + (-15j) , 5 + (-21j) , 13 + (-14j) , 8 + (-14j) , 4 + (-14j) , 4 + (-10j) , 9 + (-11j) , 8 + (-3j) , 13 + (0j) , 11 + (5j) , 10 + (11j) , 14 + (9j) , 14 + (15j) , 21 + (13j) ,],


[ -9 + (30j) , -14 + (25j) , -15 + (18j) , -10 + (19j) , -11 + (15j) , -8 + (13j) , -9 + (8j) , -16 + (7j) , -12 + (3j) , -22 + (0j) , -20 + (-3j) , -28 + (-3j) , -28 + (-1j) , -28 + (-1j) , -30 + (-3j) , -28 + (-2j) , -29 + (1j) , -32 + (3j) , -25 + (6j) , -19 + (6j) , -20 + (6j) , -13 + (4j) , -16 + (5j) , -17 + (2j) , -9 + (-4j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 7 + (11j) , 9 + (5j) , 3 + (3j) , 3 + (8j) , -8 + (12j) , 13 + (14j) , -3 + (18j) , -1 + (18j) , 6 + (28j) , 9 + (26j) , 7 + (25j) , 6 + (17j) , 13 + (17j) , 19 + (18j) , 20 + (17j) , 18 + (16j) , 13 + (14j) , 3 + (12j) , 2 + (14j) , 1 + (13j) , -6 + (14j) , -9 + (16j) , -14 + (11j) , -10 + (22j) , -20 + (20j) , -18 + (24j) ,],


[ -13 + (31j) , -12 + (37j) , -12 + (25j) , -8 + (26j) , 2 + (21j) , -13 + (17j) , -6 + (9j) , -8 + (8j) , -12 + (1j) , -12 + (-7j) , -22 + (-7j) , -27 + (-17j) , -25 + (-8j) , -31 + (-7j) , -32 + (-16j) , -29 + (-12j) , -34 + (-11j) , -36 + (-4j) , -29 + (-3j) , -34 + (1j) , -20 + (8j) , -17 + (1j) , -9 + (-3j) , -4 + (-2j) , -12 + (-10j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 8 + (-11j) , 12 + (-11j) , 13 + (-2j) , 4 + (5j) , 8 + (3j) , 11 + (6j) , 13 + (8j) , 18 + (10j) , 20 + (8j) , 27 + (9j) , 25 + (8j) , 21 + (4j) , 24 + (4j) , 21 + (4j) , 25 + (0j) , 20 + (-3j) , 20 + (1j) , 8 + (-4j) , 10 + (5j) , -5 + (15j) , -4 + (8j) , -7 + (13j) , -6 + (14j) , -7 + (23j) , -16 + (29j) , -13 + (23j) ,],


[ 26 + (14j) , 29 + (18j) , 24 + (11j) , 21 + (8j) , 15 + (10j) , 14 + (7j) , 12 + (3j) , -2 + (8j) , -4 + (12j) , -13 + (12j) , -11 + (13j) , -20 + (15j) , -17 + (24j) , -13 + (25j) , -21 + (24j) , -13 + (19j) , -12 + (22j) , -14 + (25j) , -12 + (27j) , -3 + (23j) , -2 + (22j) , -2 + (18j) , -6 + (15j) , -5 + (4j) , -9 + (7j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -11 + (-7j) , -4 + (-10j) , -5 + (-8j) , 1 + (-4j) , 3 + (-2j) , 12 + (-9j) , 8 + (-18j) , 13 + (-12j) , 10 + (-20j) , 9 + (-14j) , 10 + (-26j) , 9 + (-27j) , 14 + (-23j) , 7 + (-18j) , 8 + (-20j) , 0 + (-21j) , 6 + (-13j) , 3 + (-11j) , 7 + (-4j) , 7 + (-3j) , 12 + (2j) , 11 + (6j) , 14 + (8j) , 16 + (10j) , 18 + (19j) , 18 + (18j) ,],


[ -8 + (-33j) , -4 + (-26j) , -6 + (-27j) , -4 + (-23j) , -2 + (-19j) , -3 + (-24j) , 8 + (-17j) , 2 + (-16j) , 6 + (-9j) , 16 + (-9j) , 13 + (-14j) , 17 + (-11j) , 23 + (-13j) , 27 + (-11j) , 20 + (-14j) , 21 + (-15j) , 23 + (-15j) , 20 + (-21j) , 22 + (-18j) , 14 + (-24j) , 13 + (-23j) , 6 + (-15j) , 9 + (-17j) , 9 + (-15j) , 16 + (-7j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -11 + (10j) , -10 + (3j) , -14 + (-8j) , -7 + (1j) , -11 + (-3j) , -11 + (-8j) , -13 + (-8j) , -18 + (-10j) , -14 + (-14j) , -23 + (-16j) , -17 + (-12j) , -30 + (-14j) , -24 + (-9j) , -27 + (-6j) , -22 + (-5j) , -19 + (-7j) , -19 + (-7j) , -19 + (-10j) , -12 + (1j) , -8 + (-11j) , 2 + (-10j) , -8 + (-20j) , 0 + (-18j) , 7 + (-19j) , 0 + (-25j) , 4 + (-32j) ,],


[ 27 + (15j) , 26 + (20j) , 21 + (12j) , 25 + (10j) , 16 + (6j) , 10 + (7j) , 3 + (10j) , 2 + (7j) , 0 + (11j) , -11 + (8j) , -14 + (9j) , -21 + (13j) , -21 + (16j) , -23 + (18j) , -22 + (16j) , -18 + (23j) , -21 + (27j) , -19 + (23j) , -17 + (21j) , -18 + (17j) , -16 + (21j) , -11 + (13j) , -3 + (13j) , -13 + (8j) , -7 + (6j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -9 + (-4j) , -3 + (-7j) , -5 + (-3j) , 4 + (-7j) , -1 + (-6j) , 5 + (-11j) , 9 + (-17j) , 5 + (-22j) , 9 + (-21j) , 9 + (-19j) , 5 + (-21j) , 1 + (-28j) , 5 + (-23j) , 4 + (-24j) , -1 + (-28j) , -1 + (-15j) , 0 + (-16j) , 3 + (-11j) , 8 + (-5j) , 9 + (-1j) , 9 + (-1j) , 8 + (10j) , 11 + (7j) , 22 + (11j) , 17 + (13j) , 21 + (18j) ,],


[ 22 + (-15j) , 19 + (-24j) , 18 + (-20j) , 20 + (-14j) , 11 + (-15j) , 14 + (-11j) , 8 + (0j) , 8 + (-8j) , 14 + (4j) , 19 + (12j) , 17 + (14j) , 19 + (15j) , 25 + (13j) , 24 + (20j) , 32 + (13j) , 26 + (17j) , 29 + (15j) , 24 + (7j) , 24 + (7j) , 24 + (17j) , 21 + (4j) , 22 + (0j) , 12 + (7j) , 14 + (7j) , 8 + (8j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -19 + (-5j) , -11 + (-2j) , -2 + (-6j) , -2 + (-4j) , -4 + (-5j) , -9 + (-13j) , -5 + (-12j) , -7 + (-19j) , -12 + (-27j) , -8 + (-24j) , -13 + (-25j) , -17 + (-23j) , -17 + (-21j) , -16 + (-21j) , -12 + (-17j) , -18 + (-15j) , -15 + (-16j) , -5 + (-12j) , -2 + (-7j) , -1 + (-8j) , 4 + (-5j) , 14 + (-4j) , 11 + (-14j) , 23 + (-9j) , 18 + (-17j) , 20 + (-24j) ,],


[ -20 + (24j) , -16 + (25j) , -14 + (23j) , -8 + (18j) , -8 + (17j) , -11 + (15j) , -16 + (8j) , -16 + (-1j) , -18 + (-4j) , -16 + (3j) , -20 + (-3j) , -30 + (-9j) , -23 + (-4j) , -34 + (1j) , -30 + (2j) , -34 + (-3j) , -29 + (7j) , -27 + (4j) , -28 + (10j) , -21 + (7j) , -18 + (11j) , -14 + (3j) , -10 + (6j) , -10 + (0j) , -12 + (-5j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 6 + (12j) , 7 + (9j) , 8 + (8j) , 0 + (4j) , -2 + (9j) , 4 + (12j) , -3 + (14j) , -7 + (13j) , -3 + (20j) , 2 + (23j) , 1 + (24j) , 8 + (23j) , 10 + (26j) , 13 + (21j) , 9 + (23j) , 10 + (18j) , 6 + (12j) , 1 + (9j) , 6 + (11j) , -2 + (16j) , -8 + (12j) , -12 + (9j) , -11 + (12j) , -15 + (13j) , -17 + (23j) , -23 + (23j) ,],


[ 18 + (-19j) , 21 + (-17j) , 22 + (-23j) , 19 + (-17j) , 21 + (-15j) , 22 + (-14j) , 11 + (-7j) , 15 + (-7j) , 16 + (-7j) , 16 + (-2j) , 18 + (-2j) , 20 + (9j) , 19 + (5j) , 26 + (4j) , 26 + (8j) , 29 + (5j) , 27 + (1j) , 22 + (1j) , 30 + (0j) , 25 + (-4j) , 24 + (-2j) , 16 + (-5j) , 16 + (-8j) , 14 + (-7j) , 10 + (-7j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -1 + (-17j) , -4 + (-13j) , -1 + (-16j) , -9 + (-13j) , -2 + (-12j) , 0 + (-12j) , -2 + (-19j) , 4 + (-22j) , 4 + (-19j) , 1 + (-19j) , -6 + (-23j) , 5 + (-26j) , -2 + (-22j) , -5 + (-15j) , 0 + (-23j) , -5 + (-24j) , 3 + (-18j) , 1 + (-18j) , 3 + (-18j) , 7 + (-16j) , 6 + (-11j) , 5 + (-16j) , 12 + (-13j) , 13 + (-7j) , 21 + (-15j) , 11 + (-16j) ,],


[ 21 + (20j) , 20 + (11j) , 18 + (15j) , 19 + (14j) , 24 + (7j) , 17 + (8j) , 10 + (7j) , 10 + (14j) , 5 + (14j) , 7 + (7j) , 8 + (22j) , 5 + (16j) , 1 + (24j) , 9 + (27j) , 8 + (20j) , 10 + (28j) , 14 + (21j) , 10 + (25j) , 13 + (21j) , 15 + (16j) , 9 + (19j) , 15 + (8j) , 13 + (13j) , 17 + (17j) , 8 + (13j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 13 + (3j) , 12 + (-1j) , 10 + (-3j) , 11 + (4j) , 11 + (7j) , 13 + (5j) , 14 + (-1j) , 20 + (7j) , 18 + (4j) , 23 + (4j) , 16 + (7j) , 25 + (5j) , 23 + (-6j) , 27 + (-6j) , 18 + (4j) , 16 + (0j) , 10 + (-3j) , 12 + (-1j) , 10 + (3j) , 15 + (4j) , 10 + (5j) , 13 + (10j) , 10 + (11j) , 13 + (16j) , 16 + (14j) , 14 + (15j) ,],


[ -4 + (20j) , -4 + (27j) , -3 + (29j) , 2 + (23j) , 0 + (19j) , -5 + (18j) , -5 + (11j) , -16 + (4j) , -19 + (14j) , -13 + (8j) , -25 + (10j) , -17 + (3j) , -26 + (10j) , -29 + (10j) , -23 + (16j) , -29 + (14j) , -28 + (15j) , -21 + (18j) , -20 + (22j) , -19 + (21j) , -12 + (15j) , -12 + (9j) , -12 + (14j) , -10 + (16j) , -17 + (7j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 8 + (6j) , 10 + (5j) , 8 + (6j) , 10 + (3j) , 10 + (10j) , 5 + (15j) , 4 + (11j) , 4 + (16j) , 11 + (20j) , 3 + (21j) , 16 + (16j) , 17 + (22j) , 19 + (20j) , 13 + (16j) , 13 + (16j) , 15 + (18j) , 9 + (9j) , 8 + (12j) , 2 + (13j) , 2 + (12j) , -1 + (6j) , -8 + (14j) , -7 + (13j) , -3 + (21j) , -10 + (19j) , -7 + (22j) ,],


[ -21 + (3j) , -21 + (4j) , -32 + (6j) , -21 + (4j) , -20 + (0j) , -12 + (1j) , -17 + (-12j) , -18 + (-4j) , -13 + (-3j) , -17 + (-3j) , -19 + (-12j) , -13 + (-12j) , -22 + (-14j) , -16 + (-16j) , -17 + (-18j) , -18 + (-7j) , -14 + (-16j) , -30 + (-16j) , -21 + (-6j) , -21 + (-3j) , -25 + (-5j) , -20 + (-4j) , -18 + (-8j) , -17 + (-4j) , -13 + (-9j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -7 + (16j) , -14 + (13j) , -5 + (2j) , -6 + (7j) , -9 + (8j) , -16 + (3j) , -15 + (7j) , -18 + (5j) , -19 + (7j) , -19 + (11j) , -18 + (11j) , -17 + (16j) , -14 + (17j) , -12 + (13j) , -13 + (14j) , -17 + (19j) , -9 + (14j) , -12 + (12j) , -6 + (12j) , -8 + (8j) , -14 + (-1j) , -13 + (2j) , -17 + (2j) , -20 + (-2j) , -22 + (-3j) , -28 + (1j) ,],


[ 16 + (25j) , 20 + (26j) , 15 + (17j) , 12 + (18j) , 11 + (15j) , 3 + (8j) , 5 + (-3j) , 5 + (-7j) , 0 + (-12j) , 3 + (-16j) , 4 + (-26j) , 10 + (-29j) , 7 + (-30j) , 12 + (-33j) , 18 + (-33j) , 16 + (-34j) , 15 + (-32j) , 12 + (-29j) , 18 + (-18j) , 7 + (-16j) , 11 + (-9j) , 4 + (-3j) , 2 + (2j) , -3 + (7j) , -8 + (13j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 5 + (-19j) , 4 + (-15j) , 0 + (-20j) , -5 + (-11j) , -9 + (-7j) , -12 + (-4j) , -16 + (-2j) , -19 + (1j) , -21 + (14j) , -24 + (13j) , -26 + (13j) , -27 + (14j) , -23 + (11j) , -23 + (15j) , -24 + (12j) , -24 + (11j) , -17 + (9j) , -12 + (9j) , -10 + (11j) , -2 + (9j) , 7 + (7j) , 3 + (9j) , 18 + (9j) , 25 + (12j) , 26 + (23j) , 27 + (22j) ,],


[ -36 + (-10j) , -27 + (-13j) , -24 + (-12j) , -26 + (-6j) , -14 + (-7j) , -11 + (-8j) , -3 + (-5j) , 0 + (12j) , -2 + (14j) , 1 + (17j) , 4 + (20j) , 5 + (29j) , 9 + (30j) , 2 + (35j) , 1 + (37j) , -5 + (36j) , -4 + (33j) , -5 + (31j) , -7 + (25j) , -9 + (20j) , -5 + (16j) , -6 + (10j) , -9 + (-2j) , -7 + (-6j) , -2 + (-11j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 4 + (18j) , 2 + (17j) , 3 + (9j) , 12 + (5j) , 9 + (1j) , 9 + (-3j) , 13 + (-15j) , 7 + (-19j) , 7 + (-20j) , 20 + (-19j) , 18 + (-21j) , 12 + (-19j) , 13 + (-28j) , 11 + (-27j) , 14 + (-26j) , 4 + (-17j) , 6 + (-19j) , 6 + (-12j) , -2 + (-12j) , -10 + (-15j) , -11 + (-1j) , -10 + (-2j) , -26 + (-3j) , -27 + (-5j) , -32 + (-10j) , -28 + (-4j) ,],


[ -24 + (12j) , -18 + (9j) , -17 + (8j) , -8 + (9j) , -10 + (8j) , -6 + (7j) , -3 + (8j) , 1 + (11j) , 10 + (11j) , 10 + (16j) , 11 + (18j) , 9 + (22j) , 9 + (27j) , 5 + (21j) , 8 + (28j) , 6 + (26j) , 0 + (25j) , 4 + (25j) , -4 + (14j) , -6 + (15j) , 2 + (10j) , 1 + (-4j) , 1 + (-1j) , 1 + (-13j) , 2 + (-13j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -2 + (-17j) , -1 + (-14j) , -9 + (-4j) , -12 + (-15j) , -13 + (-17j) , -16 + (-3j) , -26 + (-15j) , -20 + (-12j) , -26 + (-6j) , -24 + (-4j) , -23 + (-8j) , -30 + (-9j) , -21 + (-8j) , -18 + (-4j) , -21 + (-3j) , -14 + (-3j) , -13 + (0j) , -14 + (1j) , -12 + (2j) , -8 + (14j) , -12 + (18j) , -9 + (15j) , -11 + (18j) , -16 + (15j) , -18 + (21j) , -21 + (16j) ,],


[ -5 + (-16j) , -8 + (-10j) , 0 + (-5j) , 2 + (-1j) , 2 + (4j) , 5 + (3j) , 7 + (11j) , 3 + (14j) , 12 + (16j) , 7 + (18j) , 7 + (19j) , 9 + (17j) , 7 + (19j) , 11 + (16j) , 3 + (18j) , 3 + (11j) , 6 + (10j) , 5 + (3j) , 1 + (3j) , 5 + (0j) , -5 + (-2j) , -3 + (-6j) , -5 + (-9j) , -13 + (-11j) , -7 + (-5j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -4 + (21j) , -6 + (23j) , 1 + (30j) , -1 + (30j) , 2 + (33j) , 3 + (34j) , 7 + (33j) , 6 + (30j) , 6 + (31j) , 0 + (18j) , 1 + (20j) , -5 + (14j) , -5 + (12j) , -2 + (5j) , -6 + (4j) , -6 + (-9j) , -11 + (-10j) , -10 + (-13j) , -12 + (-18j) , -14 + (-20j) , -13 + (-24j) , -10 + (-28j) , -15 + (-27j) , -11 + (-29j) , -10 + (-23j) , -12 + (-22j) ,],


[ 4 + (8j) , 0 + (8j) , -5 + (3j) , -2 + (-1j) , -7 + (-7j) , -6 + (-8j) , -5 + (-14j) , -9 + (-18j) , -7 + (-16j) , -4 + (-17j) , -5 + (-19j) , -8 + (-21j) , 1 + (-17j) , -6 + (-16j) , -6 + (-16j) , -5 + (-12j) , -7 + (-16j) , -2 + (-8j) , -4 + (-2j) , -6 + (-2j) , 3 + (4j) , 2 + (10j) , 6 + (4j) , 12 + (9j) , 15 + (9j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 5 + (-21j) , 4 + (-21j) , -1 + (-26j) , 0 + (-34j) , 0 + (-39j) , -4 + (-33j) , -10 + (-33j) , -4 + (-30j) , -9 + (-23j) , -3 + (-24j) , -4 + (-24j) , -2 + (-16j) , -4 + (-15j) , 2 + (-6j) , 5 + (-1j) , 6 + (-1j) , 9 + (5j) , 7 + (13j) , 12 + (17j) , 9 + (19j) , 11 + (27j) , 12 + (27j) , 12 + (23j) , 11 + (27j) , 9 + (28j) , 10 + (25j) ,],


[ 33 + (-13j) , 18 + (-4j) , 21 + (-3j) , 19 + (2j) , 9 + (2j) , 11 + (4j) , 4 + (15j) , 3 + (22j) , 6 + (14j) , 9 + (19j) , 7 + (22j) , 5 + (35j) , 13 + (27j) , 20 + (26j) , 10 + (14j) , 12 + (14j) , 11 + (10j) , 9 + (12j) , 4 + (10j) , -5 + (4j) , -3 + (6j) , -4 + (29j) , -6 + (12j) , -7 + (31j) , -5 + (33j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 5 + (-32j) , 2 + (-31j) , -7 + (-20j) , 6 + (-21j) , 6 + (-21j) , 8 + (-15j) , 6 + (-18j) , 9 + (-15j) , 11 + (-26j) , 12 + (-16j) , 11 + (-27j) , 6 + (-22j) , 18 + (-25j) , 11 + (-31j) , 10 + (-28j) , 3 + (-28j) , 6 + (-20j) , 3 + (-20j) , 2 + (-17j) , 12 + (-10j) , 14 + (-8j) , 19 + (2j) , 20 + (8j) , 32 + (0j) , 31 + (-1j) , 33 + (3j) ,],


[ -25 + (5j) , -35 + (-1j) , -14 + (-7j) , -15 + (4j) , -13 + (0j) , -9 + (-3j) , -5 + (-10j) , -5 + (-21j) , -1 + (-21j) , -10 + (-26j) , -9 + (-27j) , -11 + (-22j) , -12 + (-16j) , -16 + (-17j) , -18 + (-23j) , -13 + (-14j) , -12 + (-14j) , -13 + (-16j) , 0 + (-8j) , -1 + (-13j) , 8 + (-9j) , 7 + (-11j) , 9 + (-17j) , 5 + (-19j) , 1 + (-25j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -8 + (15j) , -1 + (24j) , -17 + (19j) , -6 + (18j) , -11 + (16j) , -17 + (16j) , -16 + (15j) , -17 + (18j) , -24 + (14j) , -23 + (21j) , -25 + (14j) , -22 + (24j) , -24 + (23j) , -18 + (20j) , -19 + (24j) , -8 + (19j) , -6 + (13j) , -6 + (15j) , -16 + (8j) , -11 + (4j) , -14 + (-8j) , -20 + (0j) , -18 + (-10j) , -23 + (-12j) , -28 + (-2j) , -29 + (-7j) ,],


[ 19 + (-10j) , 20 + (-16j) , 15 + (-12j) , 16 + (-9j) , 6 + (-9j) , 10 + (4j) , 2 + (6j) , 2 + (8j) , 7 + (15j) , 12 + (12j) , 12 + (20j) , 12 + (22j) , 15 + (21j) , 16 + (18j) , 17 + (15j) , 13 + (9j) , 11 + (9j) , 1 + (11j) , 2 + (3j) , 3 + (7j) , -11 + (2j) , -13 + (9j) , -8 + (12j) , -14 + (18j) , -7 + (29j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , 0 + (0j) , -14 + (-17j) , -8 + (-14j) , -5 + (-17j) , -11 + (-16j) , -8 + (-15j) , -9 + (-17j) , -3 + (-20j) , -5 + (-11j) , -8 + (-24j) , -3 + (-23j) , -1 + (-24j) , -3 + (-25j) , -12 + (-23j) , -7 + (-23j) , -7 + (-22j) , -6 + (-21j) , -5 + (-20j) , 2 + (-22j) , -2 + (-8j) , 5 + (-7j) , 6 + (-3j) , 10 + (-4j) , 22 + (-10j) , 23 + (-6j) , 26 + (-11j) , 18 + (-7j) ,],

 
]


############

# for_c = dat[0]
# for c in for_c:
#     print('{ %d, %d },'% (c.real, c.imag))

# s1 = fftpack.ifft(for_c)

# print(s1)

# sys.exit(0)

# #######




record_complex = []

for d in dat:
    amp = fftpack.ifft(d)

    record_complex.append( np.abs(amp) )

    print(amp)

np_record_complex = np.mat(record_complex)

print(np_record_complex)

fig, axes = plt.subplots( 8,8 )

## np_record_complex.shape[1] 矩阵列数
for col in range(0, np_record_complex.shape[1]):    
    abs_amp = np_record_complex[:, col]

    ##filter
    for i in range(0, len(abs_amp)):
        if abs_amp[i] < 1:
            abs_amp[i] = 0

    mpl.rcParams['font.sans-serif'] = ['SimHei']   #显示中文
    mpl.rcParams['axes.unicode_minus']=False       #显示负号

    fig_row = col//8
    fig_col = col % 8
    axes[fig_row, fig_col].plot(abs_amp, "*", ls = '-')   

    plt.title('original amp {}'.format(col))

plt.show()
