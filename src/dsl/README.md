# Neuro DSL

Neuro uses a Domain-Specific Language to define Neural Networks of Pushdown Automatons.

There are two main parts to a `.neu` file; a set of Neurons and a set of Links.

## Neurons

A Neuron is created with the keyword `neuron` followed by a globally unique identifier, followed by a block of state definitions.

A state is created with the keyword `state` followed by a locally unique identifier, followed by a block of actions.

An action is created with the keyword `action` followed by an optional pattern, followed by a block of instructions.

For example;
```
neuron 0 {
    state 0 {
        action {
            sub
            copy
            jez $end
            goto 1
            $end
        }
    }
    state 1 {
        action 0 {
            drop
            # This is a comment
            goto 0
        }
        action {
            send
        }
    }
}

neuron 1 {
    state 0 {
        action {
            swap
            drop # Another comment
        }
    }
}

neuron 2 {
    state 0 {
        action {
            goto 1
        }
    }
    state 1 {
        action {
            push 1
            add
            send
        }
    }
}
```

## Links

A unidirectional Link is established between two Neurons with the keyword `link` followed by the ID of the source and destination Neurons.

For example, to create a Link from Neuron 0 to Neurons 1 and 2 use;

```
link 0 {1 2}
```
