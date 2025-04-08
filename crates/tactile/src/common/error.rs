// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use std::io;

/// Provides Tactile error codes.
#[repr(u8)]
#[derive(Copy, Clone, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum Error {
  Unknown,
  BadParam,
  BadState,
  InvalidOp,
  OutOfRange,
  CouldNotCompress,
  CouldNotDecompress,
  Io(io::ErrorKind),
}

impl From<io::Error> for Error {
  fn from(value: io::Error) -> Self {
    Self::Io(value.kind())
  }
}
