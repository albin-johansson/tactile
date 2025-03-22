// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

// See https://doc.rust-lang.org/rustc/lints/listing/allowed-by-default.html
#![warn(absolute_paths_not_starting_with_crate)]
#![warn(ambiguous_negative_literals)]
#![warn(unnameable_types)]
#![warn(unused_qualifications)]
#![warn(unused_results)]
#![warn(missing_abi)]
#![warn(missing_copy_implementations)]
#![warn(non_ascii_idents)]
#![warn(redundant_imports)]
#![warn(redundant_lifetimes)]
#![warn(variant_size_differences)]

extern crate derive_more;

pub mod core;
pub mod tile;
