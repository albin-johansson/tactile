# Command API

The undo/redo support is provided using a typical command pattern architecture, where all commands are implemented as derived classes of the `ACommand` abstract class. All command implementations must provide `undo()`, `redo()`, and `id()` functions. Additionally, the command stack is able to merge consecutive commands of the same type for command types that override `merge_with()`. See the in-source documentation for more details regarding these functions.

## Guidelines

* Never store entity identifiers in commands, use dedicated `UUID` identifiers instead.
* Use `id()` to perform initial type checks when overriding `merge_with()`, but use `dynamic_cast` and not `static_cast` for the actual downcast.
* Most logic code should be put in one of the many systems, commands should be kept as lean as possible

## Adding a new command

The process of adding a new command to the application always involves a set of steps which are listed in this section.

1. Add an enumerator for the new command to the `CommandId` enum.
2. Implement the new command class, make sure the `id()` function returns the newly added enumerator value.
3. Most commands are executed as a result of an event, so make sure that `Application` has a function that handles the
   event.
4. Make sure the dispatcher connects the event to the appropriate `Application` function,
   see `application_events.cpp`.
5. Implement the creation of the command in one of the document classes.
