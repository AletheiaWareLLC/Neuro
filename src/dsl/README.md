# Neuro DSL

Neuro uses a Domain-Specific Language to define Neural Networks of Pushdown Automaton.

There are two main parts to a `.neu` file; a set of Neurons and a set of Connections.

## Neurons

A Neuron is created with the keyword `neuron` followed by a globally unique identifier, followed by a block of state definitions.

A state is created with the keyword `state` followed by a locally unique identifier, followed by a block of receivers.

A receiver is created with the keyword `receive` followed by an optional pattern, followed by a block of instructions.

For example;
```
neuron 0 {
    state 0 {
        receive {
            sub
            copy
            jez #end
            goto 1
            #end
        }
    }
    state 1 {
        receive 0 {
            drop
            goto 0
        }
        receive {
            send
        }
    }
}

neuron 1 {
    state 0 {
        receive {
            swap
            drop
        }
    }
}
```

## Connections

A unidirectional Connection is established between to Neurons with the keyword `connect` followed by the ID of the source and destination Neurons.

For example, to create a Connection from Neuron 0 to Neuron 1 use;

```
connect 0 1
```
