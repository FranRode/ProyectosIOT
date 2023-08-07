from dash import Dash, dcc, html, Input, Output
import paho.mqtt.publish as publish
import plotly.graph_objects as go

# Instancia de objeto Dash
app = Dash(__name__)

# Datos broker
MQTT_BROKER = "test.mosquitto.org"

app.layout = html.Div([

    html.Div(children='Regular intensidad de LED RGB: ',style={'font-size': 50,'text-align': 'center'}),
    html.Br(),
    html.Div(id='valor-rojo', style={'font-size': 25}),
    # Control que va desde 0 a 10, con pasos continuos, y el valor por defecto es 0
    dcc.Slider(
        id='slider-rojo',
        min=0,
        max=10,
        step=0.1,
        value=0,
        marks={i: f'{i}' for i in range(11)}
    ),
    html.Br(),
    html.Div(id='valor-verde', style={'font-size': 25}),
    dcc.Slider(
        id='slider-verde',
        min=0,
        max=10,
        step=0.1,
        value=0,
        marks={i: f'{i}' for i in range(11)}
    ),
    html.Br(),
    html.Div(id='valor-azul', style={'font-size': 25}),
    dcc.Slider(
        id='slider-azul',
        min=0,
        max=10,
        step=0.1,
        value=0,
        marks={i: f'{i}' for i in range(11)}
    ),
    html.Br(),
    
    
])



@app.callback(
    Output('valor-rojo', 'children'),
    [Input('slider-rojo', 'value')]
)
def update_output(value):
    publish.single('datos/ledRGB/rojo', value, hostname=MQTT_BROKER)
    return f'Porcentaje de color rojo: {value}'



@app.callback(
    Output('valor-verde', 'children'),
    [Input('slider-verde', 'value')]
)
def update_output(value):
    publish.single('datos/ledRGB/verde', value, hostname=MQTT_BROKER)
    return f'Porcentaje de color verde: {value}'

@app.callback(
    Output('valor-azul', 'children'),
    [Input('slider-azul', 'value')]
)
def update_output(value):
    publish.single('datos/ledRGB/azul', value, hostname=MQTT_BROKER)
    return f'Porcentaje de color azul: {value}'


if __name__ == '__main__':
    app.run_server(host="0.0.0.0", port="8050", debug=True)
