/*
 *
 * cfgfs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libnn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libnn.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "test.h"
#include "cfg.h"

#define BASE "data/test0"
//#define BASE "data"

int run(void)
{
	unlink(BASE "/floating-point");

	cfg_base(BASE);
	cfg_float("floating-point", 13.125f);

	return !(cfg_float("floating-point", 0.f) == 13.125f);
}

TEST_BEGIN
	.name = "Create and set default  Test",
	.description = "Saves a value into a file, then retrieves it",
	.run = run,
TEST_END
