# Tactile

This is the core module for the Tactile editor, providing the application logic and GUI. The heart of the Tactile
application is, not very surprisingly, the `Application` class, which connects the core model logic and the GUI.

The core logic is implemented using an ECS architecture, provided in the `core` folder. All systems are located in
the `tactile::sys` namespace. An important aspect of the core logic is that it has no dependencies on the GUI. Instead,
the core logic and the GUI communicate through the use of events, which are then handled by the
aforementioned `Application` class.

Another design principle followed throughout the codebase is that the GUI _never_ directly modifies the core model.
Instead, the GUI only emits events to let the model know about user input. As a result, all GUI functions only ever
accepts `const&` parameters of core model types such as
`document_model` and `entt::registry`. This design has many advantages, such as it lets us keep the GUI code as simple
as possible, having the GUI simply propagate the input it receives as events to the core application which then responds
to the inputs as appropriate.
