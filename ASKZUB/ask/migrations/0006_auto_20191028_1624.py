# Generated by Django 2.2.6 on 2019-10-28 13:24

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ask', '0005_auto_20191028_1611'),
    ]

    operations = [
        migrations.AlterField(
            model_name='author',
            name='avatar',
            field=models.ImageField(default='images/avatars/default_avatar.svg', upload_to='images/avatars/True'),
        ),
    ]
