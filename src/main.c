#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define SENSOR_NODE_ESQ DT_ALIAS(sensor_esq)
#define SENSOR_NODE_DIR DT_ALIAS(sensor_dir)

static const struct gpio_dt_spec sensor_esq =
    GPIO_DT_SPEC_GET(SENSOR_NODE_ESQ, gpios);

static const struct gpio_dt_spec sensor_dir =
    GPIO_DT_SPEC_GET(SENSOR_NODE_DIR, gpios);

int main(void)
{
    int config_sensor_esq, config_sensor_dir;

    // Verifica se a porta GPIO está pronta
    if (!device_is_ready(sensor_esq.port)) {
        printk("GPIO Sensor Esq não está pronto\n");
        return 0;
    }

    if (!device_is_ready(sensor_dir.port)) {
        printk("GPIO Sensor Dir não está pronto\n");
        return 0;
    }

    // Configura como entrada
    config_sensor_esq = gpio_pin_configure_dt(&sensor_esq, GPIO_INPUT);
    if (config_sensor_esq < 0) {
        printk("Erro ao configurar pino Sensor Esq\n");
        return 0;
    }

    config_sensor_dir = gpio_pin_configure_dt(&sensor_dir, GPIO_INPUT);
    if (config_sensor_dir < 0) {
        printk("Erro ao configurar pino Sensor Dir\n");
        return 0;
    }

    while (1) {
        int val_sensor_esq = gpio_pin_get_dt(&sensor_esq);
        int val_sensor_dir = gpio_pin_get_dt(&sensor_dir);
        printk("Valor do sensor_esq: %d\n", val_sensor_esq);
        printk("Valor do sensor_dir: %d\n", val_sensor_dir);
        k_msleep(500);
    }
}